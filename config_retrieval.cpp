#include "config_retrieval.h"
#include <iostream>
#include <fstream>

ConfigRetrieval::ConfigRetrieval() {
  config_dir = "./memhier/trace.config";
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
  std::string current_section;

  for (std::string line : parameters) {
    // determine if working in a section

    // break the string in half based on the : delimiter
    size_t delimiter_pos = line.find(":");
    if (delimiter_pos == std::string::npos) continue; // skip if no colon

    std::string key = line.substr(0, delimiter_pos);
    std::string value = line.substr(delimiter_pos+1);

    config_parameters[key] = value;
  }
}

std::unordered_map<std::string, std::string> ConfigRetrieval::get_config_mapping() {
  return config_parameters;
}