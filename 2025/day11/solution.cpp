#include <string>
#include <set>
#include "../../util/registry.hpp"
#include "../../util/util.hpp"
using namespace std;

// ---- Day-specific solutions ----

static string solvePart1(const string& input) {
    // Parse input to build adjacency list
    map<string, vector<string>> graph;
    auto lines = util::split(input, '\n');

    for (const auto& line : lines) {
        if (line.empty()) continue;

        auto parts = util::split(line, ':');
        if (parts.size() != 2) continue;

        string device = util::trim(parts[0]);
        auto outputs = util::split(parts[1], ' ');

        for (const auto& output : outputs) {
            string trimmed = util::trim(output);
            if (!trimmed.empty()) {
                graph[device].push_back(trimmed);
            }
        }
    }

    // Count all paths from "you" to "out" using DFS
    int pathCount = 0;
    function<void(const string&, set<string>)> dfs = [&](const string& current, set<string> visited) {
        if (current == "out") {
            pathCount++;
            return;
        }

        // Avoid cycles by tracking visited nodes in this path
        if (visited.count(current)) {
            return;
        }

        visited.insert(current);

        if (graph.find(current) != graph.end()) {
            for (const auto& next : graph[current]) {
                dfs(next, visited);
            }
        }
    };

    dfs("you", {});
    return to_string(pathCount);
}

static string solvePart2(const string& input) {
    // Parse input to build adjacency list
    map<string, vector<string>> graph;
    auto lines = util::split(input, '\n');

    for (const auto& line : lines) {
        if (line.empty()) continue;

        auto parts = util::split(line, ':');
        if (parts.size() != 2) continue;

        string device = util::trim(parts[0]);
        auto outputs = util::split(parts[1], ' ');

        for (const auto& output : outputs) {
            string trimmed = util::trim(output);
            if (!trimmed.empty()) {
                graph[device].push_back(trimmed);
            }
        }
    }

    // Count paths from "svr" to "out" that visit both "dac" and "fft"
    // Use memoization to cache results for (node, visitedDac, visitedFft) states
    map<string, long long> memo;

    function<long long(const string&, bool, bool)> dfs = [&](const string& current, bool visitedDac, bool visitedFft) -> long long {
        if (current == "out") {
            // Only count if both dac and fft were visited
            return (visitedDac && visitedFft) ? 1 : 0;
        }

        // Create memoization key: node + dac flag + fft flag
        string key = current + (visitedDac ? '1' : '0') + (visitedFft ? '1' : '0');
        if (memo.count(key)) {
            return memo[key];
        }

        long long total = 0;

        // Update visited flags
        bool newVisitedDac = visitedDac || (current == "dac");
        bool newVisitedFft = visitedFft || (current == "fft");

        if (graph.find(current) != graph.end()) {
            for (const auto& next : graph[current]) {
                total += dfs(next, newVisitedDac, newVisitedFft);
            }
        }

        memo[key] = total;
        return total;
    };

    long long result = dfs("svr", false, false);

    return to_string(result);
}

// ---- Auto-registration ----

struct AutoRegister2025Day11 {
    AutoRegister2025Day11() {
        registerSolution(2025, 11, solvePart1, solvePart2);
    }
};

static AutoRegister2025Day11 regInstance;
