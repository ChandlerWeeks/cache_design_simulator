#include "simulator.h"
#include <iostream>
#include <cmath>
#include <cstdint>

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

uint32_t processAddress(std::string hexString, Cache dataCache) {
  std::cout << hexString << std::endl;
  uint32_t address = hexToUint32(hexString);
  printBits(address);
  return address;
}

void processInstructions(TraceReciever instructions, Cache dataCache) {
  while(!instructions.isQueueEmpty()) {
    if (instructions.isNextRead()) {
      uint32_t address = processAddress(instructions.getRecentInstruction()[1], dataCache);
      dataCache.cacheRead(address);
    } else { // write
      uint32_t address = processAddress(instructions.getRecentInstruction()[1], dataCache);
      dataCache.cacheWrite(address);
    }
  }
}

Simulator::Simulator(Cache dataCache, Cache L2Cache, PageTable pageTable, TLB tlb, TraceReciever instructions, bool useVirtualAddresses, bool useTLB, bool useL2Cache) {
  this->useVA = useVirtualAddresses;
  this->useTLB = useTLB;
  this->useL2 = useL2Cache;
  Simulator::print_hierarchy_setup(dataCache, L2Cache, pageTable, tlb);
  processInstructions(instructions, dataCache);
}