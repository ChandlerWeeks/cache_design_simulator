# Compiler Settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Project Definitions
EXECUTABLE = memhier_sim

# Source files
SOURCES = cache.cpp \
          CacheLine.cpp \
          config_retrieval.cpp \
          main.cpp \
          PageTable.cpp \
          PageTableEntry.cpp \
          simulator.cpp \
          TLB.cpp \
          TLBEntry.cpp \
          trace_retrieval.cpp \
          statistics.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)


.DEFAULT_GOAL := all 

.PHONY: all
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

.PHONY: execute
execute: all
	./$(EXECUTABLE)
	$(MAKE) clean

.PHONY: clean
clean:
	rm -f $(OBJECTS)
	rm -f *.gcno *.gcda
