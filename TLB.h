#pragma once
#include <cmath>
#include <iostream>
#include <cstdint>

class TLB {
  private:
    uint32_t numSets;
    uint32_t setSize;
    uint32_t indexBits;
    uint32_t valid;
    uint32_t PFN;
  public:
    TLB(uint32_t numSets, uint32_t setSize);
    void printTLBInfo();
};
