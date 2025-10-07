#pragma once
#include <cmath>
#include <iostream>
#include <cstdint>
#include <vector>

class TLBEntry {
  private:
    uint32_t tag;
    uint32_t index;
    bool valid;
    uint32_t PFN;
    uint32_t timestamp;
  public:
    TLBEntry(uint32_t tag, uint32_t index, bool valid, uint32_t PFN, uint32_t timestamp);

    uint32_t getTag();
    void setTag(uint32_t tag);
    uint32_t getIndex();
    void setIndex(uint32_t index);
    bool isValid();
    void setValid(bool valid);
    uint32_t getPFN();
    void setPFN(uint32_t PFN);
    void setTimestamp(uint32_t timestamp);
    uint32_t getTimestamp();
};

class TLB {
  private:
    uint32_t numSets;
    uint32_t setSize;
    uint32_t indexBits;
    uint32_t offsetBits;
    uint32_t tagBits;
    uint32_t valid;
    uint32_t PFN;
    uint32_t timestamp;
    std::vector<std::vector<TLBEntry>> entries;
  public:
    TLB(uint32_t numSets, uint32_t setSize, uint32_t offsetBits);
    void printTLBInfo();
    void initializeTLB();
    uint32_t translateAddress(uint32_t virtualAddress, short &TLBres, uint32_t &TLBTag, uint32_t &TLBIndex, uint32_t &PFN);
    void addEntry(uint32_t virtualAddress, uint32_t PFN);
    // Invalidate any TLB entries mapped to the given physical frame (PFN)
    void invalidateByPFN(uint32_t pfn);
    void evictEntry(uint32_t virtualAddress);
};
