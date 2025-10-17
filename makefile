CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

EXECUTABLE = memhier

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

OBJECTS = $(SOURCES:.cpp=.o)

.DEFAULT_GOAL := all 

.PHONY: all
all: build run clean

.PHONY: build
build: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

.PHONY: run
run: $(EXECUTABLE)
	./$(EXECUTABLE) < test.dat

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
