#pragma once
#include <string>
#include <map>
#include <functional>
#include <utility>

struct Solution {
    std::function<std::string(const std::string&)> part1;
    std::function<std::string(const std::string&)> part2;
};

// Global accessor
std::map<std::pair<int, int>, Solution>& getRegistry();

// Helper to register solutions
void registerSolution(int year, int day,
                      std::function<std::string(const std::string&)> part1,
                      std::function<std::string(const std::string&)> part2);
