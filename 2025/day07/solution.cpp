#include <string>
#include "../../util/registry.hpp"
#include "../../util/util.hpp"
#include <unordered_set>
using namespace std;

// ---- Day-specific solutions ----

static string solvePart1(const string& input) {
    vector<string> grid = util::splitLines(input);
    int solution = 0;

    // Find index of starting position
    int i;
    for (i = 0; i < (int)grid[0].size(); i++) {
        if (grid[0][i] == 'S') break;
    }

    unordered_set<int> beams;
    beams.insert(i); // Start column
    
    // Iterate down grid and split beams
    for (int row = 1; row < (int)grid.size(); row++) {
        unordered_set<int> nextBeams;

        for (int col : beams) {
            if (col < 0 || col >= (int)grid[row].size()) continue;

            if (grid[row][col] == '^') {
                nextBeams.insert(col - 1);
                nextBeams.insert(col + 1);
                solution++;
            } else {
                nextBeams.insert(col);
            }
        }
        beams = std::move(nextBeams);
    }
    return to_string(solution);
}

static string solvePart2(const string& input) {
    vector<string> grid = util::splitLines(input);

    // Find index of starting position
    int i;
    for (i = 0; i < (int)grid[0].size(); i++) {
        if (grid[0][i] == 'S') break;
    }

    // column -> number of timelines at this column
    unordered_map<int, long long> timelines;
    timelines[i] = 1; // Start column

    // Iterate down the grid
    for (int row = 1; row < (int)grid.size(); row++) {
        unordered_map<int, long long> next;

        for (auto& [col, count] : timelines) {
            if (col < 0 || col >= (int)grid[row].size()) continue;

            if (grid[row][col] == '^') {
                // quantum split: timelines branch
                next[col - 1] += count;
                next[col + 1] += count;
            } else {
                // empty space: continue straight
                next[col] += count;
            }
        }
        timelines = std::move(next);
    }

    long long solution = 0;
    for (auto& [_, count] : timelines) {
        solution += count;
    }
    return to_string(solution);
}

// ---- Auto-registration ----

struct AutoRegister2025Day7 {
    AutoRegister2025Day7() {
        registerSolution(2025, 7, solvePart1, solvePart2);
    }
};

static AutoRegister2025Day7 regInstance;
