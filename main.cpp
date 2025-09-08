#include "config_retrieval.h"
#include "simulator.h"


int main() {
    ConfigRetrieval config_retriever;
    std::vector<std::string> results = config_retriever.get_configuration();

    // Now use the mapping
    Simulator sim(config_retriever.get_config_mapping());
}

int init_TLB() {
  return 1;
}

int init_DTLB() {
  return 1;
}

int init_PT() {
  return 1;
}

int init_data_cache() {
  return 1;
}

int init_L2() {
  return 1;
}