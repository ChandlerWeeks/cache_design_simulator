#include "simulator.h"
#include <iostream>
#include <cmath>

Simulator::Simulator(std::unordered_map<std::string, std::string> config_parameters) {
  Simulator::print_hierarchy_setup(config_parameters);
}

void Simulator::print_hierarchy_setup(std::unordered_map<std::string, std::string> config_parameters) {
  // --- TLB ---
  int tlb_sets     = std::stoi(config_parameters.at("TLB sets"));
  int tlb_set_size = std::stoi(config_parameters.at("TLB set size"));
  int index_bits   = static_cast<int>(std::log2(tlb_sets));
  
  std::cout << "Data TLB contains " << config_parameters["Number of sets"] << " sets." << "\n";
  std::cout << "Each set contains " << config_parameters["Set size"] << " entries." << "\n";
  std::cout << "Number of bits used for the index is " << "?" << "." << "\n";
  std::cout << "\n";

  std::cout << "Number of virtual pages is" << config_parameters["Number of virtual pages"] << ".\n";
  std::cout << "Number of physical pages is " << config_parameters["Number of physical pages"] << ".\n";
  std::cout << "Each page contains " << " bytes.\n";
  std::cout << "Number of bits used for the page table index is " << ".\n";
  std::cout << "Number of bits used for the page offset is " << ".\n";
  std::cout << "\n";

  std::cout << "D-cache contains " << " sets.\n"
}