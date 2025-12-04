#pragma once
#include <string>
#include <vector>

namespace util {
    std::string readFile(const std::string& path);
    std::vector<std::string> splitLines(const std::string& s);
    std::vector<std::string> split(const std::string& s, char delim);
    std:: string trim(const std::string& s);
}
