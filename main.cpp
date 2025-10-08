#include "config_retrieval.h"
#include "trace_retrieval.h"
#include <stdexcept>
#include "simulator.h"
#include "cache.h"
#include "PageTable.h"
#include "TLB.h"

//Data Translation Lookaside Buffer, cache for virtual memory address translations to physical memory
TLB init_TLB(ConfigRetrieval configRetriever, uint32_t offsetBits) {
  TLB tlb{
    static_cast<uint16_t>(std::stoi(configRetriever.get_config_mapping().at("TLB Number of sets"))),
    static_cast<uint16_t>(std::stoi(configRetriever.get_config_mapping().at("TLB Set size"))),
    offsetBits
  };
  return tlb;
}

// Page table, stores translations of virtual addresses to physical addresses
PageTable init_PT(ConfigRetrieval configRetriever) {
  PageTable pageTable{
    static_cast<uint16_t>(std::stoi(configRetriever.get_config_mapping().at("Page Table Number of virtual pages"))),
    static_cast<uint16_t>(std::stoi(configRetriever.get_config_mapping().at("Page Table Number of physical pages"))),
    static_cast<uint16_t>(std::stoi(configRetriever.get_config_mapping().at("Page Table Page size")))
    };
  return pageTable;
};

// L1 Cache, smaller but faster cache
Cache init_data_cache(ConfigRetrieval configRetriever) {
    Cache dataCache{
        static_cast<uint16_t>(std::stoi(configRetriever.get_config_mapping().at("Data Cache Number of sets"))),
        static_cast<uint16_t>(std::stoi(configRetriever.get_config_mapping().at("Data Cache Set size"))),
        static_cast<uint16_t>(std::stoi(configRetriever.get_config_mapping().at("Data Cache Line size"))),
        configRetriever.get_config_mapping().at("Data Cache Write through/no write allocate") == " y",
        "D-"
    };
  return dataCache;
}

// L2 Cache, bigger but slower
Cache init_L2(ConfigRetrieval configRetriever) {
      Cache L2Cache{
        static_cast<uint32_t>(std::stoi(configRetriever.get_config_mapping().at("L2 Cache Number of sets"))),
        static_cast<uint32_t>(std::stoi(configRetriever.get_config_mapping().at("L2 Cache Set size"))),
        static_cast<uint32_t>(std::stoi(configRetriever.get_config_mapping().at("L2 Cache Line size"))),
        configRetriever.get_config_mapping().at("L2 Cache Write through/no write allocate") == " y",
        "L2"
    };
  return L2Cache;
}

int main() {
    ConfigRetrieval configRetriever;
    TraceReciever instructions;
    
    std::vector<std::string> results = configRetriever.get_configuration();

    Cache dataCache = init_data_cache(configRetriever);
    Cache L2Cache = init_L2(configRetriever);
    PageTable pageTable = init_PT(configRetriever);
    TLB tlb = init_TLB(configRetriever, pageTable.getBitsPerPageOffset());

    // Now use the mapping
    bool useVA = configRetriever.get_config_mapping().at("L2 Cache Virtual addresses") == " y";
    bool useTLB = configRetriever.get_config_mapping().at("L2 Cache TLB") == " y";
    bool useL2 = configRetriever.get_config_mapping().at("L2 Cache L2 cache") == " y";
    Simulator sim(dataCache, L2Cache, pageTable, tlb, instructions, useVA, useTLB, useL2);
}