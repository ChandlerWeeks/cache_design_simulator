#pragma once
#include "L2.h"
#include "statistics.h"
#include <cstdint>
#include <vector>
#include <math.h>
#include <iostream>
#include "cache.h" 

class DataCache {
  public:
    DataCache(uint32_t numSets, uint32_t setSize, uint32_t lineSize, bool writeThrough, bool useL2);
    void setL2Cache(L2Cache* l2Cache);
    void setStats(Statistics* stats) { this->stats = stats; }
    void printCacheProperties();
    void extract_address_properties(uint32_t address, uint32_t* offset, uint32_t* index, uint32_t* tag);
    void cacheRead(uint32_t address, uint32_t &indexOut, uint32_t &tagOut, short &res);
    void cacheWrite(uint32_t address, uint32_t &indexOut, uint32_t &tagOut, short &res);
    void invalidateFromPFN(uint32_t PFN);
    void invalidateAddress(uint32_t address);
    bool isWriteThrough() { return writeThrough; }
    bool hasL2() { return useL2; }
  private:
    uint32_t numSets;
    uint32_t setSize;
    uint32_t lineSize;
    uint32_t cacheSize;
    uint32_t blockOffsetSize;
    uint32_t indexSize;
    uint32_t tagSize;
    bool writeThrough;
    bool useL2;
    L2Cache* l2Cache;
    uint64_t timestamp;
    Statistics* stats;
    std::vector<std::vector<CacheLine>> cache;

    void determineCacheProperties();
    void initializeCacheStructure();
};