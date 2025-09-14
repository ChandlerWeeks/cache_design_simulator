#include "config_retrieval.h"
#include "simulator.h"
#include "cache.h"

int main() {
    ConfigRetrieval config_retriever;
    std::vector<std::string> results = configRetriever.get_configuration();

    // Now use the mapping
    Simulator sim(configRetriever.get_config_mapping());
}

//Data Translation Lookaside Buffer, cache for virtual memory address translations to physical memory
int init_DTLB() {
  return 1;
}

// Page table, stores translations of virtual addresses to physical addresses
int init_PT() {
  return 1;
}

// L1 Cache, smaller but faster cache
int init_data_cache(ConfigRetrieval configRetriever) {
  L1Cache dataCache(
    configRetriever[]
  )
}

// L2 Cache, bigger but smaller
int init_L2() {
  return 1;
}