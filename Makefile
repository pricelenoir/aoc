CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = aoc
OBJ_DIR = obj

# Find all solution.cpp files in year/day directories
SOLUTION_SRCS = $(wildcard */day*/solution.cpp)
UTIL_SRCS = $(wildcard util/*.cpp)
MAIN_SRC = main.cpp

# All source files
SRCS = $(MAIN_SRC) $(UTIL_SRCS) $(SOLUTION_SRCS)

# Generate object file names
MAIN_OBJ = $(OBJ_DIR)/main.o
UTIL_OBJS = $(patsubst util/%.cpp,$(OBJ_DIR)/util_%.o,$(UTIL_SRCS))
SOLUTION_OBJS = $(patsubst %/day%/solution.cpp,$(OBJ_DIR)/solution_%_%.o,$(SOLUTION_SRCS))

# All object files
OBJS = $(MAIN_OBJ) $(UTIL_OBJS) $(SOLUTION_OBJS)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Main object file
$(OBJ_DIR)/main.o: main.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Util object files
$(OBJ_DIR)/util_%.o: util/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Solution object files (e.g., 2025/day01/solution.cpp -> obj/solution_2025_01.o)
$(OBJ_DIR)/solution_%_%.o: %/day%/solution.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create obj directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
