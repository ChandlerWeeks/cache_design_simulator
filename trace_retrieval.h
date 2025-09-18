#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <array>

class TraceReciever {
  public:
    TraceReciever();
    void getInstructions();
    std::array<std::string, 2> getRecentInstruction();
    bool isQueueEmpty();
    bool isNextRead();
  private:
    std::string tracePath;
    std::queue<std::array<std::string, 2>> instructionQueue;
    void setTraceConfiguration(std::vector<std::string> actions);
};