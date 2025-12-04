#include <iostream>
#include <string>
#include "util/registry.hpp"
#include "util/util.hpp"

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: ./aoc <year> <day>\n";
        return 1;
    }

    int year = std::stoi(argv[1]);
    int day  = std::stoi(argv[2]);

    auto& reg = getRegistry();
    auto it = reg.find({year, day});

    if (it == reg.end()) {
        std::cerr << "Solution for " << year << " day " << day << " is not registered.\n";
        return 1;
    }

    char buf[16];
    std::snprintf(buf, sizeof(buf), "day%02d", day);
    std:: string dayDir = std::to_string(year) + "/" + buf;

    std::string inputPath = dayDir + "/input.txt";

    std::string input = util::readFile(inputPath);

    if (input.empty()) {
        std::cerr << "Input file is empty: " << inputPath << "\n";
        return 1;
    }

    const Solution& solution = it->second;

    std::cout << "=== Advent of Code " << year << " Day " << day << " ===\n\n";
    std::cout << "Part 1: " << solution.part1(input) << "\n";
    std::cout << "Part 2: " << solution.part2(input) << "\n";
    return 0;
}
