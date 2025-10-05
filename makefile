#-----------------------------------------------------------------------------
# Makefile for the C++ simulator project
# This Makefile is designed to:
# 1. Compile all .cpp files into a single executable (simulator_app).
# 2. Run the executable.
# 3. Clean up the generated object files and the executable.
#-----------------------------------------------------------------------------

# Compiler Settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Project Definitions
EXECUTABLE = simulator_app

# Source files
SOURCES = cache.cpp \
          CacheLine.cpp \
          config_retrieval.cpp \
          main.cpp \
          PageTable.cpp \
          PageTableEntry.cpp \
          simulator.cpp \
          TLB.cpp \
          trace_retrieval.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

#-----------------------------------------------------------------------------
# Targets
#-----------------------------------------------------------------------------

# Default: build, run, and clean
.PHONY: execute
execute: all
	@echo "--- Running $(EXECUTABLE) ---"
	./$(EXECUTABLE)
	@echo "--- Finished Execution. Cleaning up... ---"
	$(MAKE) clean

# Build executable
.PHONY: all
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@echo "Linking executable: $@"
	$(CXX) $(OBJECTS) -o $@

# Compile each source file
%.o: %.cpp
	@echo "Compiling $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run without cleaning
.PHONY: run
run: $(EXECUTABLE)
	@echo "--- Running $(EXECUTABLE) ---"
	./$(EXECUTABLE)

# Clean all build files
.PHONY: clean
clean:
	@echo "Cleaning up object files and executable..."
	rm -f $(EXECUTABLE) $(OBJECTS)
	rm -f *.gcno *.gcda

# Clean only executable
.PHONY: clean-exe
clean-exe:
	@echo "Removing executable only..."
	rm -f $(EXECUTABLE)
