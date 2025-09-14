#pragma once
#include <string>
#include <unordered_map>
#include "cache.h"

class Simulator {
  public:
    Simulator(L1Cache dataCache, L2Cache L2Cache);
  private:
    void print_hierarchy_setup(L1Cache dataCache, L2Cache L2Cache);
};