#pragma once
#include <iostream>
#include <cstdint>

class PageTable {
  private:
    uint32_t virtualPageCount;
    uint32_t physicalPagesCount;
    uint32_t pageSize;
    uint32_t bitsPerPTI;
    uint32_t bitsPerPO;
};