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

  initializePageTable();
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
  std::cout << "VPN calculated as " << vpn << std::endl;
  PageTableEntry* entry = nullptr;

  // determine if VPN is mapped
  std::cout << "address size " << entries.size() << std::endl;
  if (vpn < entries.size()) {
    entry = &entries[vpn];
    if (entry->isValid()) {
      entry->setTimestamp(timestamp++);
      return (entry->getPFN() << bitsPerPageOffset) | (virtualAddress & (pageSize - 1));
    } else {
      // page table miss
      std::cout << "hierarchy: PT miss for vpn " << addressToHex(vpn) << std::endl;
      handleSoftPageFault(entry, virtualAddress);
      entry->setValid(true);
      entry->setTimestamp(timestamp++);
      return (entry->getPFN() << bitsPerPageOffset) | (virtualAddress & (pageSize - 1));
    }
  }

  // hard page fault, shits fucked...
  throw std::runtime_error("hierarchy: vpn " + addressToHex(vpn) + " is too large");
}

void PageTable::handleSoftPageFault(PageTableEntry* entry, uint32_t virtualAddress) {
  if (pfnUsedCount < physicalPageCount) {
    entry->setPFN(pfnUsedCount++);
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