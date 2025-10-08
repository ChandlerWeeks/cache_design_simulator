#include "TLB.h"

TLB::TLB(uint32_t numSets, uint32_t setSize, uint32_t offsetBits) {
  this->numSets = numSets;
  this->setSize = setSize;
  this->indexBits = log2(numSets);
  this->offsetBits = offsetBits;
  this->tagBits = 32 - indexBits - offsetBits;
  this->timestamp = 0;
  initializeTLB();
}

void TLB::printTLBInfo() {
  std::cout << "Data TLB contains " << this->numSets << " sets.\n";
  std::cout << "Each set contains " << this->setSize << " entries.\n";
  std::cout << "Number of bits used for the index is " << this->indexBits << ".\n";
}

void TLB::initializeTLB() {
  entries.resize(numSets); // dimension 1

  // set each block size;
  for (uint32_t i=0; i < numSets; ++i) {
    entries[i].resize(setSize, TLBEntry(0, i, false, 0, -1));
  }
}

uint32_t TLB::translateAddress(uint32_t virtualAddress, short &TLBres, uint32_t &TLBTag, uint32_t &TLBIndex, uint32_t &PFN) {
  uint32_t tag = virtualAddress >> (indexBits + offsetBits);
  uint32_t index = (virtualAddress >> offsetBits) & ((1 << indexBits) - 1);
  TLBTag = tag;
  TLBIndex = index;

  // get the set
  std::vector<TLBEntry>& set = entries[index];
  // check each entry in the set for a hit
  for (long unsigned int i = 0; i < set.size(); i++) {
    TLBEntry& entry = set[i];
    if (entry.isValid() && entry.getTag() == tag) {
      // TLB hit
      stats->incrementDTLBHits();
      TLBres = 1;
      entry.setTimestamp(timestamp++);
      PFN = entry.getPFN();
      return (entry.getPFN() << offsetBits) | (virtualAddress & ((1 << offsetBits) - 1));
    }
  }

  // TLB miss
  TLBres = false;
  stats->incrementDTLBMisses();
  return 0;
}

void TLB::addEntry(uint32_t virtualAddress, uint32_t PFN) {
  uint32_t tag = virtualAddress >> (indexBits + offsetBits);
  uint32_t index = (virtualAddress >> offsetBits) & ((1 << indexBits) - 1);

  std::vector<TLBEntry>& set = entries[index];
  // Find the oldest entry 
  int lruIndex = 0;
  uint32_t oldestTimestamp = set[0].getTimestamp();
  for (long unsigned int i = 0; i < set.size(); i++) {
    if (!set[i].isValid()) {
      lruIndex = i;
      break;
    }
    if (set[i].getTimestamp() < oldestTimestamp) {
      oldestTimestamp = set[i].getTimestamp();
      lruIndex = i;
    }
  }

  // Replace the LRU entry with the new mapping
  set[lruIndex] = TLBEntry(tag, index, true, PFN, timestamp++);
}

void TLB::evictEntry(uint32_t virtualAddress) {
  uint32_t tag = virtualAddress >> (indexBits + offsetBits);
  uint32_t index = (virtualAddress >> offsetBits) & ((1 << indexBits) - 1);

  std::vector<TLBEntry>& set = entries[index];
  for (long unsigned int i = 0; i < set.size(); i++) {
    TLBEntry& entry = set[i];
    if (entry.isValid() && entry.getTag() == tag) {
      entry.setValid(false);
      return;
    }
  }
}

void TLB::invalidateByPFN(uint32_t pfn) {
  for (uint32_t si = 0; si < numSets; ++si) {
    for (uint32_t ei = 0; ei < setSize; ++ei) {
      TLBEntry &entry = entries[si][ei];
      if (entry.isValid() && entry.getPFN() == pfn) {
        entry.setValid(false);
      }
    }
  }
}