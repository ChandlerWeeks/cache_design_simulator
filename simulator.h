#pragma once
#include <string>
#include <unordered_map>
#include "cache.h"
#include "trace_retrieval.h"
#include "PageTable.h"
#include "TLB.h"

class Simulator {
  public:
    Simulator(Cache dataCache, Cache L2Cache, PageTable pageTable, TLB tlb, TraceReciever instructions, bool useVirtualAddresses, bool useTLB, bool useL2Cache);
  private:
    bool useL2;
    bool useVA;
    bool useTLB;
    void print_hierarchy_setup(Cache dataCache, Cache L2cache, PageTable pageTable, TLB tlb);
    void printHeader();
    void printOutputRow();
    void processInstructions(TraceReciever instructions, Cache dataCache);
};