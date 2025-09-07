#include "DataReciever.h"
#include <iostream>
#include <fstream>

DataReciever::DataReciever() {
  config_dir = "./memhier/trace.config";
  trace_path = "./memhier/trace.dat";
}

std::vector<std::string> DataReciever::get_configuration() {
  std::vector<std::string> results;
  std::ifstream file(config_dir);

  if (!file.is_open()) {
    std::cout << "Shit's broke";
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

void DataReciever::set_config_parameters(std::vector<std::string> parameters) {
  for (std::string line : parameters) {
    // break the string in half based on the : delimiter
    size_t delimiter_pos = line.find(":");
    if (delimiter_pos == std::string::npos) continue; // skip if no colon

    std::string key = line.substr(0, delimiter_pos);
    std::string value = line.substr(delimiter_pos+1);

    config_parameters[key] = value;
  }
}