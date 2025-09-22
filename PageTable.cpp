#include "PageTable.h"

PageTable::PageTable(uint32_t virtualPageCount, uint32_t physicalPageCount, uint32_t pageSize) {
  this->virtualPageCount = virtualPageCount;
  this->physicalPageCount = physicalPageCount;
  this->pageSize = pageSize;
  this->bitsPerPTI = log2(virtualPageCount);
  this->bitsPerPageOffset = log2(pageSize);
}

void PageTable::printPTAttributes() {
  std::cout << "Number of virtual pages is " << this->virtualPageCount << ".\n";
  std::cout << "Number of physical pages is " << this->physicalPageCount << ".\n";
  std::cout << "Each page contains " << this->pageSize << " bytes.\n";
  std::cout << "Number of bits used for the page table index is " << this->bitsPerPTI << ".\n";
  std::cout << "Number of bits used for the page offset is " << this->bitsPerPageOffset << ".\n";
}