#include "PageTable.h"

std::string addressToHex(uint32_t address) {
  std::ostringstream oss;
  oss << std::hex << std::nouppercase << address;
  return oss.str();
}

PageTable::PageTable(uint32_t virtualPageCount, uint32_t physicalPageCount, uint32_t pageSize) {
  this->virtualPageCount = virtualPageCount;
  this->physicalPageCount = physicalPageCount;
  this->pageSize = pageSize;
  this->bitsPerPTI = log2(virtualPageCount);
  this->bitsPerPageOffset = log2(pageSize);
  this->pfnUsedCount = 0;
  this->timestamp = 0;
}

void PageTable::initializePageTable() {
  // because there can be different amounts of vp to pp create it pte manually
  for (uint32_t i = 0; i < virtualPageCount; i++) {
    PageTableEntry page(i, 0, 0, false, false);
    entries.push_back(page);
  }
}

uint32_t PageTable::translateAddress(uint32_t virtualAddress) {
  uint32_t vpn = virtualAddress >> bitsPerPageOffset;
  uint32_t offsetMask = (1 << bitsPerPageOffset) - 1;
  uint32_t offset = virtualAddress & offsetMask;
  PageTableEntry* entry = nullptr;

  // determine if VPN is mapped
  for (long unsigned int i = 0; i < entries.size(); i++) { // for whatever god forsaken reason, it didn't like "int"
    if (entries[i].getIndex() == vpn) {
      timestamp++;
      entry = &entries[i];

      if (!entry->isValid()) {
        handleSoftPageFault(entry, virtualAddress);
      } if (!entry->isDirty()) {
        // page table is dirty reference the disk (change counter)
      }
      // yay, we got it and its not being picky <3
      entry->setTimestamp(timestamp);
    }
  }

  // hard page fault, shits fucked...
  std::cout << std::endl << "hierarchy: virtual address " << addressToHex(virtualAddress) << " is too large" << std::endl;
  throw std::runtime_error("skibidi ohio rizz");
}

void PageTable::handleSoftPageFault(PageTableEntry* entry, uint32_t virtualAddress) {
  if (pfnUsedCount < physicalPageCount) {

  } else {
    evictLRU(virtualAddress);
  }
}

void PageTable::printPTAttributes() {
  std::cout << "Number of virtual pages is " << this->virtualPageCount << ".\n";
  std::cout << "Number of physical pages is " << this->physicalPageCount << ".\n";
  std::cout << "Each page contains " << this->pageSize << " bytes.\n";
  std::cout << "Number of bits used for the page table index is " << this->bitsPerPTI << ".\n";
  std::cout << "Number of bits used for the page offset is " << this->bitsPerPageOffset << ".\n";
}

void PageTable::evictLRU(uint32_t virtualAddress) {
  PageTableEntry* evictedEntry = findLRU();
  if (evictedEntry->isDirty()) {
    // disk write
  }
  uint32_t vpn = virtualAddress >> bitsPerPageOffset;
  PageTableEntry replacement(vpn, evictedEntry->getPFN(), timestamp, true, false);
  *evictedEntry = replacement;
}

PageTableEntry* PageTable::findLRU() {
  PageTableEntry* oldest = &entries[0];
  for (long unsigned int i = 0; i < entries.size(); i++) {
    if (!entries[i].isValid()) {
      return &entries[i];
    }
    if (oldest->getTimestamp() > entries[i].getTimestamp()) {
      oldest = &entries[i];
    }
  }
  return oldest;
}