#include "config_retrieval.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

ConfigRetrieval::ConfigRetrieval() {
  for (auto& entry : std::filesystem::directory_iterator("./")) {
    if (entry.path().filename() == "trace.config") {
      config_dir = entry.path().string();
      break;
    }
  }
}

std::vector<std::string> ConfigRetrieval::get_configuration() {
  std::vector<std::string> results;
  std::ifstream file(config_dir);

  if (!file.is_open()) {
    std::cout << "Error reading config file.\n";
    return results;
  }

  // Read the contents of the config file
  std::string line;
  while (std::getline(file, line)) {
    results.push_back(line);
  }

  file.close();

  set_config_parameters(results);

  return results;
}

// Set a mapping of keys to values for each part of the simulation
void ConfigRetrieval::set_config_parameters(std::vector<std::string> parameters) {
  std::string currSection;

  for (std::string line : parameters) {
    if (line == "Data TLB configuration") {
      currSection = "TLB ";
    } else if (line == "Page Table configuration") {
      currSection = "Page Table ";
    } else if (line == "Data Cache configuration") {
      currSection = "Data Cache ";
    } else if (line == "L2 Cache configuration") {
      currSection = "L2 Cache ";
    }

    // break the string in half based on the : delimiter
    size_t delimiter_pos = line.find(":");
    if (delimiter_pos == std::string::npos) continue; // skip if no colon

    std::string key = line.substr(0, delimiter_pos);
    std::string value = line.substr(delimiter_pos+1);

    config_parameters[currSection + key] = value;
  }
}

std::unordered_map<std::string, std::string> ConfigRetrieval::get_config_mapping() {
  return config_parameters;
}