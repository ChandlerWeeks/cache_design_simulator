#include "simulator.h"
#include <iostream>
#include <cmath>
#include <cstdint>
#include <cstdio>

void Simulator::print_hierarchy_setup(Cache dataCache, Cache L2cache, PageTable pageTable, TLB tlb) {
  if (useTLB) {
    tlb.printTLBInfo();
  } std::cout << std::endl;
  if (useVA) {
    pageTable.printPTAttributes();
  } std::cout << std::endl;
  dataCache.printCacheProperties();
  std::cout << std::endl;
  if (useL2) {
    std::cout << std::endl;
    L2cache.printCacheProperties();
  }std::cout << std::endl;

  if (useVA) {
    std::cout << "The addresses read in are virtual addresses";
  }
}

uint32_t hexToUint32(const std::string hexString) {
    return static_cast<uint32_t>(std::stoul(hexString, nullptr, 16));
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

void Simulator::printOutputRow() {
  printf("%-8s %-6s %-4s %-6s %-3s %-4s %-4s %-4s %-6s %-3s %-4s %-6s %-3s %-4s\n",
       "Virtual", "Virt.", "Page", "TLB", "TLB", "TLB", "PT", "Phys",
       "DC", "DC", "L2", "L2", "L2", "L2");
}

std::string boolToHitMiss(short val) {
  switch(val) {
    case -1:
      return "";
    case 0:
      return "miss";
    case 1:
      return "hit";
  }
}

//TODO: make it so this prints each row what happens
void Simulator::processInstructions(TraceReciever instructions, Cache dataCache, PageTable pageTable) {
  uint32_t address = 0, VPN = 0, pageOffset = 0,
          TLBTag = 0, TLBIndex = 0, PFN = 0,
          DCTag = 0, DCIndex = 0,
          L2Tag = 0, L2Index = 0;

  // use a short for a boolean with *3* whole states
  short TLBRes = -1, PTRes = -1, DCRes = -1, L2Res = -1;


  while(!instructions.isQueueEmpty()) {
    uint32_t virtualAddress = processAddress(instructions.getRecentInstruction()[1]);
    uint32_t physicalAddress;
    if (useTLB) {
      // TODO: implement that whole TLB thing
    }
    

    if (useVA && (!useTLB || 0==0)) { // eventually put tag for tlb hit
      physicalAddress = pageTable.translateAddress(virtualAddress);
    } else {
      physicalAddress = virtualAddress;
    }

    // data cache is NOT optional
    if (instructions.isNextRead()) {
      dataCache.cacheRead(physicalAddress);
    } else { // write
      dataCache.cacheWrite(physicalAddress);
    }

    if (useL2) {
      
    }
    printOutputRow();
  }
  
}

Simulator::Simulator(Cache dataCache, Cache L2Cache, PageTable pageTable, TLB tlb, TraceReciever instructions, bool useVirtualAddresses, bool useTLB, bool useL2Cache) {
  this->useVA = useVirtualAddresses;
  this->useTLB = useTLB;
  this->useL2 = useL2Cache;
  Simulator::print_hierarchy_setup(dataCache, L2Cache, pageTable, tlb);
  std::cout << std::endl;
  printHeader();
  processInstructions(instructions, dataCache, pageTable);
}