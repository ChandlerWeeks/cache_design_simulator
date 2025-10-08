#pragma once
#include <string>
#include <unordered_map>
#include "cache.h"
#include "trace_retrieval.h"
#include "PageTable.h"
#include "TLB.h"
#include "statistics.h"

class Simulator {
  public:
    Simulator(Cache dataCache, Cache L2Cache, PageTable pageTable, TLB tlb, TraceReciever instructions, bool useVirtualAddresses, bool useTLB, bool useL2Cache);
  private:
    bool useL2;
    bool useVA;
    bool useTLB;
    Statistics stats;
    int memoryAccesses = 0;
    int pageTableAccesses = 0;
    int diskAccesses = 0;
    void print_hierarchy_setup(Cache dataCache, Cache L2cache, PageTable pageTable, TLB tlb);
    void printHeader();
    void printOutputRow();
    void processInstructions(TraceReciever instructions, Cache dataCache, Cache L2Cache, PageTable pageTable, TLB tlb);
    void printOutputRow(uint32_t address  = 0, uint32_t VPN = 0, uint32_t pageOffset = 0,uint32_t TLBTag   = 0, uint32_t TLBIndex = 0, short TLBRes = -1,short PTRes       = -1, uint32_t PFN = 0,uint32_t DCTag    = 0, uint32_t DCIndex = 0, short DCRes = -1,uint32_t L2Tag    = 0, uint32_t L2Index = 0, short L2Res = -1);
};