#include "simulator.h"
#include <iostream>
#include <cmath>

Simulator::Simulator(L1Cache dataCache, L2Cache L2Cache) {
  Simulator::print_hierarchy_setup(dataCache, L2Cache);
}

void Simulator::print_hierarchy_setup(L1Cache dataCache, L2Cache L2cache) {
  dataCache.printCacheInfo();
  std::cout << std::endl;
  L2cache.printCacheInfo();
}