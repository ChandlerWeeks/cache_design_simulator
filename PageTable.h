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
  public:
    PageTableEntry(uint32_t index, uint32_t timestamp, bool valid, bool dirty);

    uint32_t getIndex() const;
    void setIndex(uint32_t index);

    uint32_t getTimestamp() const;
    void setTimestamp(uint32_t timestamp);

    bool isValid() const;
    void setValid(bool valid);

    bool isDirty() const;
    void setDirty(bool dirty);
};

class PageTable {
  public:
    PageTable(uint32_t virtualPageCount, uint32_t physicalPageCount, uint32_t pageSize);
    void printPTAttributes();
    void initializePageTable();
  private:
    uint32_t timestamp;
    uint32_t virtualPageCount;
    uint32_t physicalPageCount;
    uint32_t pageSize;
    uint32_t bitsPerPTI;
    uint32_t bitsPerPageOffset;
    std::vector<PageTableEntry> entries;
};