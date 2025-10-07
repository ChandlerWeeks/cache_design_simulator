# Compiler Settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Project Definitions
EXECUTABLE = CacheSimulator

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
          trace_retrieval.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

#-----------------------------------------------------------------------------
# Targets
#-----------------------------------------------------------------------------

# Default: build, run, and clean
.PHONY: execute
execute: all
	./$(EXECUTABLE)
	$(MAKE) clean

# Build executable
.PHONY: all
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

# Compile each source file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run without cleaning
.PHONY: run
run: $(EXECUTABLE)
	./$(EXECUTABLE)

# Clean all build files
.PHONY: clean
clean:
	rm -f $(OBJECTS)
	rm -f *.gcno *.gcda

# Clean only executable
#.PHONY: clean-exe
# clean-exe:
#	rm -f $(EXECUTABLE)
