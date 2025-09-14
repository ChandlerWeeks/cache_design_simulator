#include "cache.h"

// L1_cache attributes
L1Cache::L1Cache(ushort numSets, ushort setSize, uint lineSize, bool writeThrough) {
  numSets = numSets;
  setSize = setSize;
  lineSize = LineSize;
  writeThrough = writeThrough;
}

void L1Cache::determineCacheProperties() {
  cacheSize = numSets * setSize * lineSize;
  blockOffsetSize = log2(cacheSize);
  indexSize = log2(numSets);
  tagSize = 32 - indexSize - blockOffsetSize;
}