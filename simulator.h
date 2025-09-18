#pragma once
#include <string>
#include <unordered_map>
#include "cache.h"
#include "trace_retrieval.h"

class Simulator {
  public:
    Simulator(Cache dataCache, Cache L2Cache, TraceReciever instructions, bool useVirtualAddresses, bool useTLB, bool useL2Cache);
  private:
    bool useL2;
    bool useVA;
    bool useTLB;
    void print_hierarchy_setup(Cache dataCache, Cache L2Cache);
};