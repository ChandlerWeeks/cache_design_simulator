#pragma once
#include <iostream>
#include <cstdint>
#include <cmath>

class PageTable {
  public:
    PageTable(uint32_t virtualPageCount, uint32_t physicalPageCount, uint32_t pageSize);
    void printPTAttributes();
  private:
    uint32_t virtualPageCount;
    uint32_t physicalPageCount;
    uint32_t pageSize;
    uint32_t bitsPerPTI;
    uint32_t bitsPerPageOffset;
};