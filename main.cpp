#include "config_retrieval.h"
#include <stdexcept>
#include "simulator.h"
#include "cache.h"

//Data Translation Lookaside Buffer, cache for virtual memory address translations to physical memory
int init_DTLB() {
  return 1;
}

// Page table, stores translations of virtual addresses to physical addresses
int init_PT() {
  return 1;
}

// L1 Cache, smaller but faster cache
L1Cache init_data_cache(ConfigRetrieval configRetriever) {
    L1Cache dataCache{
        static_cast<uint16_t>(std::stoi(configRetriever.get_config_mapping().at("Data Cache Number of sets"))),
        static_cast<uint16_t>(std::stoi(configRetriever.get_config_mapping().at("Data Cache Set size"))),
        static_cast<uint16_t>(std::stoi(configRetriever.get_config_mapping().at("Data Cache Line size"))),
        configRetriever.get_config_mapping().at("Data Cache Write through/no write allocate") == "y"
    };
  return dataCache;
}

// L2 Cache, bigger but smaller
L2Cache init_L2(ConfigRetrieval configRetriever) {
      L2Cache L2Cache{
        static_cast<uint16_t>(std::stoi(configRetriever.get_config_mapping().at("Data Cache Number of sets"))),
        static_cast<uint16_t>(std::stoi(configRetriever.get_config_mapping().at("Data Cache Set size"))),
        static_cast<uint16_t>(std::stoi(configRetriever.get_config_mapping().at("Data Cache Line size"))),
        configRetriever.get_config_mapping().at("Data Cache Write through/no write allocate") == "y"
    };
  return L2Cache;
}

int main() {
    ConfigRetrieval configRetriever;
    std::vector<std::string> results = configRetriever.get_configuration();
    L1Cache dataCache = init_data_cache(configRetriever);
    L2Cache L2Cache = init_L2(configRetriever);

    // Now use the mapping
    Simulator sim(dataCache, L2Cache);
}