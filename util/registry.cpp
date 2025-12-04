#include "registry.hpp"

std::map<std::pair<int, int>, Solution>& getRegistry() {
    static std::map<std::pair<int, int>, Solution> instance;
    return instance;
}

void registerSolution(int year, int day,
                      std::function<std::string(const std::string&)> part1,
                      std::function<std::string(const std::string&)> part2) {
    getRegistry()[{year,day}] = {part1, part2};
}
