#pragma once
#include <iostream>
#include <cstdint>
#include <cmath>
#include <vector>
#include <math.h>
#include "statistics.h"
#include "cache.h"

class DataCache;

class L2Cache {
  public:
    L2Cache(uint32_t numSets, uint32_t setSize, uint32_t lineSize, bool writeThrough);
    void setParent(DataCache* parentCache);
    void setStats(Statistics* stats) { this->stats = stats; }
    void printCacheProperties();
    void extract_address_properties(uint32_t address, uint32_t* offset, uint32_t* index, uint32_t* tag);
    void cacheRead(uint32_t address, uint32_t &indexOut, uint32_t &tagOut, short &res);
    void cacheWrite(uint32_t address, uint32_t &indexOut, uint32_t &tagOut, short &res);
    void invalidateFromPFN(uint32_t PFN);
    void invalidateAddress(uint32_t address);
    bool isWriteThrough() { return writeThrough; }
  private:
    uint32_t numSets;
    uint32_t setSize;
    uint32_t lineSize;
    uint32_t cacheSize;
    uint32_t blockOffsetSize;
    uint32_t indexSize;
    uint32_t tagSize;
    bool writeThrough;
    uint64_t timestamp;
    Statistics* stats;
    std::vector<std::vector<CacheLine>> cache;
    DataCache* parentCache;

    void determineCacheProperties();
    void initializeCacheStructure();
};