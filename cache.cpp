#include "cache.h"
#include <iostream>

void Cache::printCacheInfo() {
  std::cout << "D-cache contains " < numSets << " sets.\n";
  std::cout << "Each set contains " < setSize < "entries.\n";
  std::cout << "Each line is " << lineSize << " bytes.\n";
  if (writeThrough) {
    std::cout << "The Cache uses a write-allocate and write back policy\n";
  } else {
    std::cout << "Uh oh\n";
  }
  std::cout << "Number of bits used for the index is " << indexSize << ".\n";
  std::cout << "Number of bits used for the offset is " << blockOffsetSize << ".\n";
}