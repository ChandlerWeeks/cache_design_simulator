#pragma once
#include <string>
#include <vector>
#include <unordered_map>

class DataReciever {
  public:
    DataReciever();
    std::vector<std::string> get_configuration();
    void set_config_parameters(std::vector<std::string> parameters);
  private:
    // file directories
    std::string config_dir;
    std::string trace_path;
    std::unordered_map<std::string, std::string> config_parameters;
};