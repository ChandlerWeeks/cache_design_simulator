#include "simulator.h"
#include <iostream>
#include <cmath>
#include <cstdint>
#include <cstdio>

void Simulator::print_hierarchy_setup(Cache dataCache, Cache L2cache, PageTable pageTable, TLB tlb) {
  tlb.printTLBInfo();
  std::cout << std::endl;
  pageTable.printPTAttributes();
  std::cout << std::endl;
  dataCache.printCacheProperties();
  std::cout << std::endl;
  L2cache.printCacheProperties();
  std::cout << std::endl;

  if (useVA) {
    std::cout << "The addresses read in are virtual addresses." << std::endl;
  } else {
    std::cout << "The addresses read in are physical addresses." << std::endl;
  }
}

uint32_t hexToUint32(const std::string hexString) {
    return static_cast<uint32_t>(std::stoul(hexString, nullptr, 16));
}

std::string addressToHex(uint32_t address) {
  std::ostringstream oss;
  oss << std::hex << std::nouppercase << address;
  return oss.str();
}

void printBits(uint32_t n) {
  for (int i = 31; i>=0; --i) {
    if ((n >> i) & 1) {
      std::cout << "1";
    } else {
      std::cout << "0";
    }
  }
  std::cout << std::endl;
}

// hex address to virtual/physical 
uint32_t processAddress(std::string hexString) {
  // std::cout << hexString << std::endl;
  uint32_t address = hexToUint32(hexString);
  // printBits(address);
  return address;
}

void Simulator::printHeader() {
  printf("Virtual  Virt.  Page TLB    TLB TLB  PT   Phys        DC  DC          L2  L2\n");
  printf("Address  Page # Off  Tag    Ind Res. Res. Pg # DC Tag Ind Res. L2 Tag Ind Res.\n");
  printf("-------- ------ ---- ------ --- ---- ---- ---- ------ --- ---- ------ --- ----\n");
}

std::string hitMissStr(short res) {
    switch (res) {
        case -1: return "";
        case 0:  return "miss";
        case 1:  return "hit";
    }
    return "";
}

void Simulator::printOutputRow(
  uint32_t address, uint32_t VPN, uint32_t pageOffset,
  uint32_t TLBTag, uint32_t TLBIndex, short TLBRes,
  short PTRes, uint32_t PFN,
  uint32_t DCTag, uint32_t DCIndex, short DCRes,
  uint32_t L2Tag, uint32_t L2Index, short L2Res)
{

  printf("%08x", address);
  if (useVA) {
    printf(" %6x %4x", VPN, pageOffset);
  }

  // TLB columns only if TLB is enabled
  if (useTLB) {
    printf(" %6x %3x %-4s", TLBTag, TLBIndex, hitMissStr(TLBRes).c_str());
  }

  // PT columns only if using VA
  if (useVA) {
    printf(" %-4s %4x", hitMissStr(PTRes).c_str(), PFN);
  }

  // DC is always on
  printf(" %6x %3x %-4s", DCTag, DCIndex, hitMissStr(DCRes).c_str());

  // L2 columns only if L2 is enabled
  if (useL2) {
    if (DCRes == 1) {
      // blank on DC hit
      printf(" %6s %3s %-4s\n", "", "", "");
    } else {
      printf(" %6x %3x %-4s\n", L2Tag, L2Index, hitMissStr(L2Res).c_str());
    }
  } else {
    // no L2 → finish the line
    printf("\n");
  }
}

std::string boolToHitMiss(short val) {
  switch(val) {
    case 0:
      return "miss";
    case 1:
      return "hit";
  }
  return "";
}

void Simulator::processInstructions(TraceReciever instructions, Cache dataCache, Cache L2Cache, PageTable pageTable, TLB tlb) {
  uint32_t address = 0, VPN = 0, pageOffset = 0,
           TLBTag = 0, TLBIndex = 0, PFN = 0,
           DCTag = 0, DCIndex = 0,
           L2Tag = 0, L2Index = 0;

  // use a short for a boolean with *3* whole states
  short TLBRes = -1, PTRes = -1, DCRes = -1, L2Res = -1;

  while(!instructions.isQueueEmpty()) {
    // get the next instruction
    bool isRead = instructions.isNextRead();
    if (isRead) {
      stats.incrementReads();
    } else {
      stats.incrementWrites();
    }
    uint32_t virtualAddress = processAddress(instructions.getRecentInstruction()[1]);
    uint32_t physicalAddress;
    Cache* lowestCache = nullptr;
    Cache* highestCache = nullptr;
    if (useL2) {
      lowestCache = &L2Cache;
      highestCache = &dataCache;
      L2Cache.setParent(&dataCache);
      dataCache.setNextLevel(&L2Cache);
    } else {
      highestCache = &dataCache;
    }

    address = virtualAddress;
    VPN = virtualAddress >> pageTable.getBitsPerPageOffset();
    pageOffset = virtualAddress & ( (1 << pageTable.getBitsPerPageOffset()) - 1);
    if (useVA && useTLB) {
      pageTable.setTLB(&tlb);
      pageTable.setLowestCache(lowestCache);
      pageTable.setHighestCache(highestCache);
    }

    if (useTLB) {
      physicalAddress = tlb.translateAddress(virtualAddress, TLBRes, TLBTag, TLBIndex, PFN);
    }
    if (useVA && (!useTLB || TLBRes != 1)) { // eventually put tag for tlb hit
      physicalAddress = pageTable.translateAddress(virtualAddress, PTRes, PFN);
    } else if (useVA && TLBRes == 1) {
      // TLB hit path:
      uint32_t vpn = virtualAddress >> pageTable.getBitsPerPageOffset();
      pageTable.incrementTimestamp(vpn); 
    }
    if (!useVA && !useTLB) {
      physicalAddress = virtualAddress;
    }

    if (useTLB && TLBRes == 0) {
      tlb.addEntry(virtualAddress, PFN);
    }

    // always use data cache
    if (isRead) {
      dataCache.cacheRead(physicalAddress, DCIndex, DCTag, DCRes);
    } else { // write
      dataCache.cacheWrite(physicalAddress, DCIndex, DCTag, DCRes);
      if (useVA) {
        pageTable.markAddressDirty(virtualAddress);
      }
    }

    if (useL2) {
      if (DCRes == 0) { // L2 only accessed on DC miss
        if (isRead) {
          L2Cache.cacheRead(physicalAddress, L2Index, L2Tag, L2Res);
        } else { // write
          L2Cache.cacheWrite(physicalAddress, L2Index, L2Tag, L2Res);
        }
      } else if (dataCache.isWriteThrough() && !isRead) {
        // on a DC hit with write-through, write to L2 as well
        uint32_t dummy1, dummy2;
        short dummy3;
        L2Cache.cacheWrite(physicalAddress, dummy1, dummy2, dummy3);

      }
    }
    printOutputRow(address, VPN, pageOffset, TLBTag, TLBIndex, TLBRes, PTRes, PFN, DCTag, DCIndex, DCRes, L2Tag, L2Index, L2Res);

    // reset everything for next iteration
    address = 0; VPN = 0; pageOffset = 0;
    TLBTag = 0; TLBIndex = 0; PFN = 0;
    DCTag = 0; DCIndex = 0;
    L2Tag = 0; L2Index = 0;
    TLBRes = -1; PTRes = -1; DCRes = -1; L2Res = -1;
  }
}

Simulator::Simulator(Cache dataCache, Cache L2Cache, PageTable pageTable, TLB tlb, TraceReciever instructions, bool useVirtualAddresses, bool useTLB, bool useL2Cache) {
  this->useVA = useVirtualAddresses;
  this->useTLB = useTLB;
  this->useL2 = useL2Cache;
  this->stats = Statistics();

  // prepare statistics
  tlb.setStats(&stats);
  pageTable.setStats(&stats);
  dataCache.setStats(&stats);
  L2Cache.setStats(&stats);

  Simulator::print_hierarchy_setup(dataCache, L2Cache, pageTable, tlb);
  std::cout << std::endl;
  printHeader();
  processInstructions(instructions, dataCache, L2Cache, pageTable, tlb);
  stats.printStatistics();
}