#include <algorithm>
#include <string>
#include <utility>
#include "../../util/registry.hpp"
#include "../../util/util.hpp"
using namespace std;

// ---- Day-specific solutions ----

static string solvePart1(const string& input) {
    vector<string> inputVec = util::splitLines(input);
    int solution = 0;
    
    vector<pair<long long,long long>> ranges;
    int i = 0;
    while(inputVec[i] != "") {
        auto dashPos = inputVec[i].find("-");
        long long start = stoll(inputVec[i].substr(0, dashPos));
        long long end = stoll(inputVec[i].substr(dashPos+1));
        ranges.push_back({start, end});
        i++;
    }
    i++; // Skip blank lines

    // Sort and merge intervals
    sort(ranges.begin(), ranges.end());
    vector<pair<long long,long long>> merged;
    for (const auto& range : ranges) {
        if (merged.empty() || range.first > merged.back().second + 1) {
            // No overlap or adjacency, add new range
            merged.push_back(range);
        } else {
            // Overlaps or adjacent to last range, extend it
            merged.back().second = max<long long>(merged.back().second, range.second);
        }
    }

    while (i < (int)inputVec.size()) {
        long long id = stoll(inputVec[i]);
        auto it = upper_bound(merged.begin(), merged.end(), make_pair(id, LLONG_MAX)); // find first range where start > id

        if (it != merged.begin()) {
            --it;
            if (id >= it->first && id <= it->second) {
                solution++;
            }
        }
        i++;
    }
    return to_string(solution);
}

static string solvePart2(const string& input) {
    vector<string> inputVec = util::splitLines(input);
    long long solution = 0;

    vector<pair<long long,long long>> ranges;
    int i = 0;
    while(i < (int)inputVec.size() && inputVec[i] != "") {
        auto dashPos = inputVec[i].find("-");
        long long start = stoll(inputVec[i].substr(0, dashPos));
        long long end = stoll(inputVec[i].substr(dashPos+1));
        ranges.push_back({start, end});
        i++;
    }

    // Sort and merge intervals
    sort(ranges.begin(), ranges.end());
    vector<pair<long long,long long>> merged;
    for (const auto& range : ranges) {
        if (merged.empty() || range.first > merged.back().second + 1) {
            // No overlap or adjacency, add new range
            merged.push_back(range);
        } else {
            // Overlaps or adjacent to last range, extend it
            merged.back().second = max<long long>(merged.back().second, range.second);
        }
    }

    // Count total IDs in all merged ranges
    for (const auto& range : merged) {
        solution += (range.second - range.first + 1);
    }
    return to_string(solution);
}

// ---- Auto-registration ----

struct AutoRegister2025Day5 {
    AutoRegister2025Day5() {
        registerSolution(2025, 5, solvePart1, solvePart2);
    }
};

static AutoRegister2025Day5 regInstance;
