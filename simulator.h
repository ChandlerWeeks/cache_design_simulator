#pragma once
#include <string>
#include <unordered_map>

class Simulator {
  public:
    Simulator(std::unordered_map<std::string, std::string> config_parameters);
  private:
    void print_hierarchy_setup(std::unordered_map<std::string, std::string> config_parameters);
};