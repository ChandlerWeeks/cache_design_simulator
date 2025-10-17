#include "config_retrieval.h"
#include "trace_retrieval.h"
#include <stdexcept>
#include "simulator.h"
#include "DC.h"
#include "L2.h"
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
DataCache init_data_cache(ConfigRetrieval configRetriever, bool useL2) {
    DataCache dataCache{
        static_cast<uint16_t>(std::stoi(configRetriever.get_config_mapping().at("Data Cache Number of sets"))),
        static_cast<uint16_t>(std::stoi(configRetriever.get_config_mapping().at("Data Cache Set size"))),
        static_cast<uint16_t>(std::stoi(configRetriever.get_config_mapping().at("Data Cache Line size"))),
        configRetriever.get_config_mapping().at("Data Cache Write through/no write allocate") == " y",
        useL2,
    };
  return dataCache;
}

// L2 Cache, bigger but slower
L2Cache init_L2(ConfigRetrieval configRetriever) {
      L2Cache l2cache{
        static_cast<uint32_t>(std::stoi(configRetriever.get_config_mapping().at("L2 Cache Number of sets"))),
        static_cast<uint32_t>(std::stoi(configRetriever.get_config_mapping().at("L2 Cache Set size"))),
        static_cast<uint32_t>(std::stoi(configRetriever.get_config_mapping().at("L2 Cache Line size"))),
        configRetriever.get_config_mapping().at("L2 Cache Write through/no write allocate") == " y"
    };
    return l2cache;
}

int main() {
    ConfigRetrieval configRetriever;
    TraceReciever instructions;
    
    std::vector<std::string> results = configRetriever.get_configuration();

    // get the bools
    bool useVA = configRetriever.get_config_mapping().at("L2 Cache Virtual addresses") == " y";
    bool useTLB = configRetriever.get_config_mapping().at("L2 Cache TLB") == " y";
    bool useL2 = configRetriever.get_config_mapping().at("L2 Cache L2 cache") == " y";

    DataCache dataCache = init_data_cache(configRetriever, useL2);
    L2Cache l2cache = init_L2(configRetriever);
    PageTable pageTable = init_PT(configRetriever);
    TLB tlb = init_TLB(configRetriever, pageTable.getBitsPerPageOffset());

    Simulator sim(dataCache, l2cache, pageTable, tlb, instructions, useVA, useTLB, useL2);
}