#pragma once
#include <cstdint>
#include <math.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "statistics.h"

class CacheLine {
  private:
    uint32_t tag;
    bool dirty;
    bool valid;
    uint64_t timestamp;
  public:
    CacheLine();
    bool isCacheHit(uint32_t tag);
    bool replaceCache(uint32_t tag, bool dirty);
    void setTimestamp(uint64_t timestamp);
    void setDirtyBit();
    void invalidate();
    uint64_t getTimestamp();
    bool isValid();
    bool isDirty();
    uint32_t getTag();
};

class Cache {
public:
    Cache(uint32_t numSets, uint32_t setSize, uint32_t lineSize, bool writeThrough, std::string type);
    uint32_t getLineSize();
    void setNextLevel(Cache* nextLevel);
    void setParent(Cache* parent);
    void printCacheProperties();
    void extract_address_properties(uint32_t address, uint32_t* offset, uint32_t* index, uint32_t* tag);
    void cacheRead(uint32_t address, uint32_t &indexOut, uint32_t &tagOut, short &res);
    void cacheWrite(uint32_t address, uint32_t &indexOut, uint32_t &tagOut, short &res);
    void invalidateAddress(uint32_t address);
    void setStats(Statistics* stats) { this->stats = stats; }
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
    std::string cacheType;
    Cache* nextLevel;
    Cache* parentCache;
    Statistics* stats;

    std::vector<std::vector<CacheLine>> cache;

    void determineCacheProperties();
    void initializeCacheStructure();
    void invalidateParents(uint32_t address);
    uint32_t reconstructAddress(uint32_t tag, uint32_t setIndex); // New helper function
};