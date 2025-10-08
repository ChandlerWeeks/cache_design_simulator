#pragma once
#include <iostream>
#include <cstdint>
#include <cmath>
#include <vector>
#include <sstream>
#include "TLB.h"
#include "cache.h"
#include "statistics.h"

class PageTableEntry {
  private:
    uint32_t index;
    uint32_t timestamp; // used for LRU
    bool valid;
    bool dirty;
    uint32_t PFN;
  public:
    PageTableEntry(uint32_t index, uint32_t PFN, uint32_t timestamp, bool valid, bool dirty);

    uint32_t getIndex();
    void setIndex(uint32_t index);
    uint32_t getTimestamp();
    void setTimestamp(uint32_t timestamp);
    bool isValid();
    void setValid(bool valid);
    bool isDirty();
    void setDirty(bool dirty);
    uint32_t getPFN();
    void setPFN(uint32_t PFN);
};

class PageTable {
  private:
    uint32_t pfnUsedCount;
    uint32_t timestamp;
    uint32_t virtualPageCount;
    uint32_t physicalPageCount;
    uint32_t pageSize;
    uint32_t bitsPerPTI;
    uint32_t bitsPerPageOffset;
    std::vector<PageTableEntry> entries;
    uint32_t evictLRU();
    uint32_t findLRU();
    TLB* tlb;
    Statistics* stats;
    Cache* highestCache; // the highest cache in the hierarchy, used for write-backs
    Cache* lowestCache; // the lowest cache in the hierarchy, used for fetching on a page fault
    void handleSoftPageFault(PageTableEntry* entry);
  public:
    PageTable(uint32_t virtualPageCount, uint32_t physicalPageCount, uint32_t pageSize);
    void printPTAttributes();
    void initializePageTable();
    uint32_t translateAddress(uint32_t virtualAddress, short &PTres, uint32_t &PFN);
    uint32_t getBitsPerPageOffset();
    void printPageTable();
    void markAddressDirty(uint32_t virtualAddress);
    void incrementTimestamp(uint32_t vpn);
    void setTLB(TLB* tlb);
    void setHighestCache(Cache* cache);
    void setLowestCache(Cache* cache);
    void setStats(Statistics* stats) { this->stats = stats; }
};