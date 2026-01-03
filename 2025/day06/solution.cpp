#include <string>
#include "../../util/registry.hpp"
#include "../../util/util.hpp"
using namespace std;

// ---- Day-specific solutions ----

static string solvePart1(const string& input) {
    auto lines = util::splitLines(input);

    vector<vector<int>> allNumbers;
    vector<char> operators;

    for (size_t i = 0; i < lines.size(); i++) {
        const auto& line = lines[i];
        auto parts = util::split(line, ' ');

        if (i < 4) {
            // First 4 lines are numbers
            vector<int> numbers;
            for (const auto& part : parts) {
                if (!part.empty()) {
                    numbers.push_back(stoi(part));
                }
            }
            allNumbers.push_back(numbers);
        } else {
            // Last line is operators
            for (const auto& part : parts) {
                if (!part.empty()) {
                    operators.push_back(part[0]);
                }
            }
        }
    }

    long long solution = 0;
    for (size_t i = 0; i < allNumbers[0].size(); i++) {
        long long val1 = allNumbers[0][i];
        long long val2 = allNumbers[1][i];
        long long val3 = allNumbers[2][i];
        long long val4 = allNumbers[3][i];
        char op = operators[i];

        long long val = 0;
        if (op == '+') {
            val = val1 + val2 + val3 + val4;
        } else {
            val = val1 * val2 * val3 * val4;
        }
        solution += val;
    }

    return to_string(solution);
}

static string solvePart2(const string& input) {
    auto lines = util::splitLines(input);

    // Build a grid where spaces are converted to '.'
    vector<string> grid;
    size_t maxLen = 0;

    for (size_t i = 0; i < lines.size() - 1; i++) {  // -1 to exclude operator line
        string line = lines[i];
        // Replace spaces with '.'
        for (auto& c : line) {
            if (c == ' ') c = '.';
        }
        maxLen = max(maxLen, line.size());
        grid.push_back(line);
    }

    // Pad lines to same length
    for (auto& line : grid) {
        while (line.size() < maxLen) {
            line += '.';
        }
    }

    // Parse operator line to find problem boundaries
    const string& operatorLine = lines[lines.size() - 1];
    vector<size_t> problemStarts;
    vector<char> operators;

    for (size_t i = 0; i < operatorLine.size(); i++) {
        if (operatorLine[i] != ' ') {
            problemStarts.push_back(i);
            operators.push_back(operatorLine[i]);
        }
    }
    problemStarts.push_back(maxLen);  // Add sentinel

    long long grandTotal = 0;

    // Process each problem from right to left
    for (int p = problemStarts.size() - 2; p >= 0; p--) {
        size_t start = problemStarts[p];
        size_t end = problemStarts[p + 1];
        char op = operators[p];

        // For this problem, extract columns and read vertically
        vector<long long> numbers;

        for (size_t col = start; col < end && col < maxLen; col++) {
            // Read this column top-to-bottom to form a number
            string numStr = "";
            for (size_t row = 0; row < grid.size(); row++) {
                if (grid[row][col] != '.') {
                    numStr += grid[row][col];
                }
            }
            if (!numStr.empty()) {
                numbers.push_back(stoll(numStr));
            }
        }

        // Calculate result for this problem
        long long result = 0;
        if (op == '+') {
            for (long long num : numbers) {
                result += num;
            }
        } else if (op == '*') {
            result = 1;
            for (long long num : numbers) {
                result *= num;
            }
        }

        grandTotal += result;
    }

    return to_string(grandTotal);
}

// ---- Auto-registration ----

struct AutoRegister2025Day6 {
    AutoRegister2025Day6() {
        registerSolution(2025, 6, solvePart1, solvePart2);
    }
};

static AutoRegister2025Day6 regInstance;
