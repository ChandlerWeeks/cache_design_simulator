#include "trace_retrieval.h"
#include <iostream>
#include <fstream>

TraceReciever::TraceReciever() {
  std::string traceLine;
  std::vector<std::string> actions;

  // Read from cin
  while (std::getline(std::cin, traceLine)) {
    if (!traceLine.empty()) {
      actions.push_back(traceLine);
    }
  }

  if (actions.empty()) {
    std::cerr << "No instructions provided in standard input." << std::endl;
  }

  // Parse and populate the instruction queue
  setTraceConfiguration(actions);
}

// extract the data from the line
void TraceReciever::setTraceConfiguration(std::vector<std::string> actions) {
  for (std::string line : actions) {
    size_t delimiter_pos = line.find(":");
    if (delimiter_pos == std::string::npos) continue;

    std::string actionType = line.substr(0, delimiter_pos);
    std::string address = line.substr(delimiter_pos+1);

    std::array<std::string, 2> instruction = {actionType, address};

    instructionQueue.push(instruction);
  }
}

// get the top instruction, and remove it from the queue
std::array<std::string, 2> TraceReciever::getRecentInstruction() {
  std::array<std::string, 2> instruction = instructionQueue.front();
  instructionQueue.pop();
  return instruction;
}

bool TraceReciever::isQueueEmpty() {
  return (instructionQueue.size() == 0);
}

bool TraceReciever::isNextRead() {
  return (instructionQueue.front()[0] == "R");
}