#include "PageTable.h"

std::string addressToHexi(uint32_t address) {
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
  this->tlb = nullptr;

  initializePageTable();
}

void PageTable::setTLB(TLB* tlb) {
  this->tlb = tlb;
}

void PageTable::initializePageTable() {
  // because there can be different amounts of vp to pp create it pte manually
  for (uint32_t i = 0; i < virtualPageCount; i++) {
    PageTableEntry page(i, 0, -1, false, false);
    entries.push_back(page);
  }
}

void PageTable::markAddressDirty(uint32_t virtualAddress) {
  uint32_t vpn = virtualAddress >> bitsPerPageOffset;
  entries[vpn].setDirty(true);
}

uint32_t PageTable::translateAddress(uint32_t virtualAddress, short &PTres, uint32_t &PFN) {
  uint32_t vpn = virtualAddress >> bitsPerPageOffset;
  PageTableEntry* entry = nullptr;

  // determine if VPN is mapped
  if (vpn < entries.size()) {
    entry = &entries[vpn];
    if (entry->isValid()) {
      PTres = 1; // hit
      entry->setTimestamp(timestamp++);
      PFN = entry->getPFN();
      return (entry->getPFN() << bitsPerPageOffset) | (virtualAddress & (pageSize - 1));
    } else {
      PTres = 0; // miss
      handleSoftPageFault(entry, virtualAddress);
      entry->setTimestamp(timestamp++);
      PFN = entry->getPFN();
      return (entry->getPFN() << bitsPerPageOffset) | (virtualAddress & (pageSize - 1));
    }
  }

  // hard page fault, shits fucked...
  throw std::runtime_error("hierarchy: virtual address " + addressToHexi(virtualAddress) + " is too large");
}

void PageTable::handleSoftPageFault(PageTableEntry* entry, uint32_t virtualAddress) {
  if (pfnUsedCount < physicalPageCount) {
    entry->setPFN(pfnUsedCount++);
    entry->setValid(true);
  } else {
    uint32_t evictedPFN = evictLRU(virtualAddress);
    entry->setPFN(evictedPFN);
    entry->setValid(true);
    // invalidat
  }
}

void PageTable::printPTAttributes() {
  std::cout << "Number of virtual pages is " << this->virtualPageCount << ".\n";
  std::cout << "Number of physical pages is " << this->physicalPageCount << ".\n";
  std::cout << "Each page contains " << this->pageSize << " bytes.\n";
  std::cout << "Number of bits used for the page table index is " << this->bitsPerPTI << ".\n";
  std::cout << "Number of bits used for the page offset is " << this->bitsPerPageOffset << ".\n";
}

uint32_t PageTable::evictLRU(uint32_t virtualAddress) {
  uint32_t evictedIndex = findLRU();
  PageTableEntry* evictedEntry = &entries[evictedIndex];
  uint32_t freedPFN = entries[evictedIndex].getPFN();
  uint32_t vpn = virtualAddress >> bitsPerPageOffset;
  if (evictedEntry->isDirty()) {
    // disk write
    evictedEntry->setDirty(false);
  }

  // invalidate pages with the same pfn
  for (PageTableEntry& entry : entries) {
    if (entry.getPFN() == freedPFN) {
      entry.setValid(false);
    }
  }


  evictedEntry->setValid(false);
  if (tlb != nullptr) {
    tlb->invalidateByPFN(freedPFN);
  }
  return freedPFN;
}

uint32_t PageTable::findLRU() {
  PageTableEntry *oldest = nullptr;
  for (long unsigned int i = 0; i < entries.size(); i++) {
    if (oldest == nullptr || entries[i].getTimestamp() < oldest->getTimestamp()) {
      if (entries[i].isValid()) {
        oldest = &entries[i];
      }
    }
  }
  return oldest->getIndex();
}

uint32_t PageTable::getBitsPerPageOffset() {
  return bitsPerPageOffset;
}

void PageTable::incrementTimestamp(uint32_t vpn) {
  if (vpn < entries.size()) {
    entries[vpn].setTimestamp(timestamp++);
  }
}

void PageTable::printPageTable() {
  std::cout << "Page Table Entries:\n";
  std::cout << "Index\tPFN\tValid\tDirty\tTimestamp\n";
  for (PageTableEntry entry : entries) {
    std::cout << entry.getIndex() << "\t" 
              << entry.getPFN() << "\t" 
              << entry.isValid() << "\t" 
              << entry.isDirty() << "\t" 
              << entry.getTimestamp() << "\n";
  }
}