#pragma once
#include <iostream>
#include <cstdint>
#include <cmath>
#include <vector>

class PageTableEntry {
  private:
    uint32_t index;
    uint32_t timestamp; // used for LRU
    bool valid;
    bool dirty;
    uint32_t PFN;
  public:
    PageTableEntry(uint32_t index, uint32_t timestamp, bool valid, bool dirty);

    uint32_t getIndex();
    void setIndex(uint32_t index);
    uint32_t getTimestamp();
    void setTimestamp(uint32_t timestamp);
    bool isValid();
    void setValid(bool valid);
    bool isDirty();
    void setDirty(bool dirty);
};

class PageTable {
  public:
    PageTable(uint32_t virtualPageCount, uint32_t physicalPageCount, uint32_t pageSize);
    void printPTAttributes();
    void initializePageTable();
    uint32_t translateAddress(uint32_t virtualAddress);
  private:
    uint32_t timestamp;
    uint32_t virtualPageCount;
    uint32_t physicalPageCount;
    uint32_t pageSize;
    uint32_t bitsPerPTI;
    uint32_t bitsPerPageOffset;
    std::vector<PageTableEntry> entries;
    uint32_t validEntryCount;
};