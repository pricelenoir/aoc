#include <string>
#include <vector>
#include <unordered_set>
#include "../../util/registry.hpp"
#include "../../util/util.hpp"
using namespace std;

// ---- Day-specific solutions ----

// Helper to convert char --> int
int ctoi(char c) {
    return c - '0';
}

static string solvePart1(const string& input) {
    vector<string> inputVec = util::splitLines(input);
    int total = 0;
    for (string str : inputVec) {
        // Make 2 passes:
        // 1. Find global max
        // 2. Find local max after global max
        int idx1 = 0;
        for (int i = 1; i < (int)str.length()-1; i++) {
            if (ctoi(str[i]) > ctoi(str[idx1])) idx1 = i;
        }
        int idx2 = idx1+1;
        for (int i = idx2; i < (int)str.length(); i++) {
            if (ctoi(str[i]) > ctoi(str[idx2])) idx2 = i;
        }
        int joltage = (ctoi(str[idx1]) * 10) + ctoi(str[idx2]);
        total += joltage;
    }
    return to_string(total);
}

static string solvePart2(const string& input) {
    vector<string> inputVec = util::splitLines(input);
    long long total = 0;
    for (string str : inputVec) {
        int numDigitsToRemove = str.length()-12;

        // Greedy approach: remove digit if next digit is larger
        while(numDigitsToRemove != 0) {
            bool removed = false;
            for (int i = 0; i < (int)str.length() - 1; i++) {
                if (str[i] < str[i+1]) {
                    str.erase(i, 1);
                    numDigitsToRemove--;
                    removed = true;
                    break;
                }
            }

            if (!removed) {
                str.erase(str.length()-1, 1);
                numDigitsToRemove--;
            }
        }
        total += stoll(str);
    }
    return to_string(total);
}

// ---- Auto-registration ----

struct AutoRegister2025Day3 {
    AutoRegister2025Day3() {
        registerSolution(2025, 3, solvePart1, solvePart2);
    }
};

static AutoRegister2025Day3 regInstance;
