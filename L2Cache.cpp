#include "cache.h"
#include <iostream>

// L1_cache attributes
L2Cache::L2Cache(uint16_t numSets, uint16_t setSize, uint16_t lineSize, bool writeThrough) {
  this->numSets = numSets;
  this->setSize = setSize;
  this->lineSize = lineSize;
  this->writeThrough = writeThrough;
  determineCacheProperties();
}

void L2Cache::determineCacheProperties() {
  cacheSize = numSets * setSize * lineSize;
  blockOffsetSize = log2(cacheSize);
  indexSize = log2(numSets);
  tagSize = 32 - indexSize - blockOffsetSize;
}

void L2Cache::printCacheInfo() {
  std::cout << "L2 Cache contains " << numSets << " sets.\n";
  std::cout << "Each set contains " << setSize << " entries.\n";
  std::cout << "Each line is " << lineSize << " bytes.\n";
  if (writeThrough) {
    std::cout << "The Cache uses a write-allocate and write back policy\n";
  } else {
    std::cout << "The Cache does not uses a write-allocate and write back policy\n";
  }
  std::cout << "Number of bits used for the index is " << indexSize << ".\n";
  std::cout << "Number of bits used for the offset is " << blockOffsetSize << ".\n";
}