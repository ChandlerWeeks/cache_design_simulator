#pragma once
#include <cstdint>
#include <math.h>
#include <vector>
#include <iostream>
#include <fstream>

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
};

class Cache {
  private:
    // Cache properties
    uint32_t address;
    uint32_t numSets;
    uint32_t setSize;
    uint32_t lineSize;
    bool writeThrough;
    uint32_t cacheSize;
    uint32_t blockOffsetSize;
    uint32_t indexSize;
    uint32_t tagSize;
    std::string cacheType;
    void determineCacheProperties();
    void extract_address_properties(uint32_t address, uint32_t* offset, uint32_t* index, uint32_t* tag);

    // for replacement 
    Cache* nextLevel;
    Cache* parentCache;

    // Cache structure
    std::vector<std::vector<CacheLine>> cache; // dimension 1 is the set, dimension 2 is the block

    // LRU related
    uint64_t timestamp;
  public:
    Cache(uint32_t numSets, uint32_t setSize, uint32_t lineSize, bool writeThrough, std::string type);
    
    void setNextLevel(Cache* nextLevel);
    void setParent(Cache* parent);
    uint32_t getLineSize();
    bool invalidateAddress(uint32_t address);
    
    bool initializeCacheStructure();
    bool cacheRead(uint32_t address, uint32_t &indexOut, uint32_t &tagOut, short &res);
    bool cacheWrite(uint32_t address);
    void printCacheProperties();

    void invalidateParents(uint32_t address);
};