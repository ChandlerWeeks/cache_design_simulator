#ifndef CACHE_H
#define CACHE_H

#include <cstdint>
#include <math.h>

class Cache {
  protected:
    uint32_t address;
    uint16_t numSets;
    uint16_t setSize;
    uint16_t lineSize;
    bool writeThrough;
    uint16_t cacheSize;
    uint16_t blockOffsetSize;
    uint16_t indexSize;
    uint16_t tagSize;
};

class L1Cache : public Cache {
  public:
    L1Cache(uint16_t numSets, uint16_t setSize, uint16_t lineSize, bool writeThrough);
    void printCacheInfo();
  private:
    void determineCacheProperties();
};

class L2Cache : public Cache {
  public:
    L2Cache(uint16_t numSets, uint16_t setSize, uint16_t lineSize, bool writeThrough);
    void printCacheInfo();
  private:
    void determineCacheProperties();
};

#endif