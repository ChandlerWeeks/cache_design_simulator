#pragma once
#include <iostream>
#include <cstdint>

class  Statistics {
private:
  uint32_t dtlbHits;
  uint32_t dtlbMisses;
  uint32_t ptHits;
  uint32_t ptMisses;
  uint32_t dcHits;
  uint32_t dcMisses;
  uint32_t l2Hits;
  uint32_t l2Misses;
  uint32_t totalReads;
  uint32_t totalWrites;
  uint32_t mainMemoryAccesses;
  uint32_t diskRefs;
public:
  Statistics();
  void incrementDTLBHits() { dtlbHits++; }
  void incrementDTLBMisses() { dtlbMisses++; }
  void incrementPTHits() { ptHits++; }
  void incrementPTMisses() { ptMisses++; }
  void incrementCacheHits(std::string cacheType);
  void incrementCacheMisses(std::string cacheType);
  void incrementReads() { totalReads++; }
  void incrementWrites() { totalWrites++; }
  void incrementMainMemoryAccesses() { mainMemoryAccesses++; }
  void incrementDiskRefs() { diskRefs++; }
  void printStatistics();
};