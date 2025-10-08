#include "statistics.h"

Statistics::Statistics() {
  this->dtlbHits = 0;
  this->dtlbMisses = 0;
  this->ptHits = 0;
  this->ptMisses = 0;
  this->dcHits = 0;
  this->dcMisses = 0;
  this->l2Hits = 0;
  this->l2Misses = 0;
  this->totalReads = 0;
  this->totalWrites = 0;
  this->mainMemoryAccesses = 0;
  this->diskRefs = 0;
}

void Statistics::incrementCacheHits(std::string cacheType) {
  if (cacheType == "D-") {
    dcHits++;
  } else if (cacheType == "L2") {
    l2Hits++;
  }
}

void Statistics::incrementCacheMisses(std::string cacheType) {
  if (cacheType == "D-") {
    dcMisses++;
  } else if (cacheType == "L2") {
    l2Misses++;
  }
}

void Statistics::printStatistics() {
  std::cout << std::endl << "Simulation statistics" << std::endl;
  std::cout << std::endl;

  std::cout << "dtlb hits        : " << dtlbHits << std::endl;
  std::cout << "dtlb misses      : " << dtlbMisses << std::endl;
  if (dtlbHits + dtlbMisses > 0) {
    printf("dtlb hit rate    : %.6f\n", (double)dtlbHits / (dtlbHits + dtlbMisses));
  } else {
    std::cout << "dtlb hit rate    : N/A" << std::endl;
  }
  std::cout << std::endl;

  std::cout << "pt hits          : " << ptHits << std::endl;
  std::cout << "pt misses        : " << ptMisses << std::endl;
  if (ptHits + ptMisses > 0) {
    printf("pt hit rate      : %.6f\n", (double)ptHits / (ptHits + ptMisses));
  } else {
    std::cout << "pt hit rate      : N/A" << std::endl;
  }
  std::cout << std::endl;

  std::cout << "dc hits          : " << dcHits << std::endl;
  std::cout << "dc misses        : " << dcMisses << std::endl;
  if (dcHits + dcMisses > 0) {
    printf("dc hit rate      : %.6f\n", (double)dcHits / (dcHits + dcMisses));
  } else {
    std::cout << "dc hit rate      : N/A" << std::endl;
  }
  std::cout << std::endl;

  std::cout << "L2 hits          : " << l2Hits << std::endl;
  std::cout << "L2 misses        : " << l2Misses << std::endl;
  if (l2Hits + l2Misses > 0) {
    printf("L2 hit rate      : %.6f\n", (double)l2Hits / (l2Hits + l2Misses));
  } else {
    std::cout << "L2 hit rate      : N/A" << std::endl;
  }
  std::cout << std::endl;

  std::cout << "Total reads      : " << totalReads << std::endl;
  std::cout << "Total writes     : " << totalWrites << std::endl;
  printf("Read/Write ratio : %.6f\n", ((double)totalReads / (totalWrites + totalReads)));

  std::cout << std::endl;
  std::cout << "main memory refs : " << mainMemoryAccesses << std::endl;
  std::cout << "page table refs  : " << ptHits + ptMisses << std::endl;
  std::cout << "disk refs        : " << diskRefs << std::endl;
}
