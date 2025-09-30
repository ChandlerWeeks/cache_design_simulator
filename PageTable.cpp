#include "PageTable.h"

PageTable::PageTable(uint32_t virtualPageCount, uint32_t physicalPageCount, uint32_t pageSize) {
  this->virtualPageCount = virtualPageCount;
  this->physicalPageCount = physicalPageCount;
  this->pageSize = pageSize;
  this->bitsPerPTI = log2(virtualPageCount);
  this->bitsPerPageOffset = log2(pageSize);
}

void PageTable::initializePageTable() {
  // because there can be different amounts of vp to pp create it pte manually
  for (int i = 0; i < virtualPageCount; i++) {
    PageTableEntry page(i, 0, false, false);
    entries.push_back(page);
  }
}

uint32_t PageTable::translateAddress(uint32_t virtualAddress) {
  uint32_t vpn = virtualAddress >> bitsPerPageOffset;
  uint32_t offset = virtualAddress * (pageSize - 1);
}

void PageTable::printPTAttributes() {
  std::cout << "Number of virtual pages is " << this->virtualPageCount << ".\n";
  std::cout << "Number of physical pages is " << this->physicalPageCount << ".\n";
  std::cout << "Each page contains " << this->pageSize << " bytes.\n";
  std::cout << "Number of bits used for the page table index is " << this->bitsPerPTI << ".\n";
  std::cout << "Number of bits used for the page offset is " << this->bitsPerPageOffset << ".\n";
}