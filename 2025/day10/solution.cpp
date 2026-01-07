#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <bitset>
#include <functional>
#include <unordered_map>
#include "../../util/registry.hpp"
#include "../../util/util.hpp"
using namespace std;

// ---- Day-specific solutions ----

struct Machine {
    vector<bool> target;  // Target indicator light configuration
    vector<vector<int>> buttons;  // Each button's list of lights it toggles
};

Machine parseMachine(const string& line) {
    Machine m;

    // Parse target configuration [.##.]
    size_t start = line.find('[');
    size_t end = line.find(']');
    string targetStr = line.substr(start + 1, end - start - 1);
    for (char c : targetStr) {
        m.target.push_back(c == '#');
    }

    // Parse buttons (x,y,z)
    size_t pos = end + 1;
    while (pos < line.size()) {
        size_t parenStart = line.find('(', pos);
        if (parenStart == string::npos) break;
        size_t parenEnd = line.find(')', parenStart);

        string buttonStr = line.substr(parenStart + 1, parenEnd - parenStart - 1);
        vector<int> button;
        stringstream ss(buttonStr);
        string num;
        while (getline(ss, num, ',')) {
            button.push_back(stoi(num));
        }
        m.buttons.push_back(button);

        pos = parenEnd + 1;
        if (line[pos] == ' ' && line[pos + 1] == '{') break;  // Stop at joltage requirements
    }

    return m;
}

// Solve system of linear equations over GF(2) using Gaussian elimination
int solveMinPresses(const Machine& m) {
    int numLights = m.target.size();
    int numButtons = m.buttons.size();

    // Create augmented matrix [A|b] where A[i][j] = 1 if button j toggles light i
    vector<vector<int>> matrix(numLights, vector<int>(numButtons + 1, 0));

    for (int i = 0; i < numLights; i++) {
        matrix[i][numButtons] = m.target[i] ? 1 : 0;  // Target state
    }

    for (int j = 0; j < numButtons; j++) {
        for (int light : m.buttons[j]) {
            matrix[light][j] = 1;
        }
    }

    // Gaussian elimination over GF(2)
    vector<int> pivotCol(numLights, -1);
    int currentRow = 0;

    for (int col = 0; col < numButtons && currentRow < numLights; col++) {
        // Find pivot
        int pivotRow = -1;
        for (int row = currentRow; row < numLights; row++) {
            if (matrix[row][col] == 1) {
                pivotRow = row;
                break;
            }
        }

        if (pivotRow == -1) continue;  // No pivot in this column

        // Swap rows
        swap(matrix[currentRow], matrix[pivotRow]);
        pivotCol[currentRow] = col;

        // Eliminate
        for (int row = 0; row < numLights; row++) {
            if (row != currentRow && matrix[row][col] == 1) {
                for (int c = 0; c <= numButtons; c++) {
                    matrix[row][c] ^= matrix[currentRow][c];
                }
            }
        }

        currentRow++;
    }

    // Check for inconsistency
    for (int row = currentRow; row < numLights; row++) {
        if (matrix[row][numButtons] == 1) {
            return -1;  // No solution
        }
    }

    // Find solution with minimum button presses
    // Free variables can be 0 or 1, try all combinations
    vector<int> freeVars;
    vector<bool> isFree(numButtons, true);
    for (int row = 0; row < currentRow; row++) {
        if (pivotCol[row] != -1) {
            isFree[pivotCol[row]] = false;
        }
    }
    for (int col = 0; col < numButtons; col++) {
        if (isFree[col]) {
            freeVars.push_back(col);
        }
    }

    int minPresses = INT_MAX;
    int numFree = freeVars.size();

    // Try all combinations of free variables
    for (int mask = 0; mask < (1 << numFree); mask++) {
        vector<int> solution(numButtons, 0);

        // Set free variables
        for (int i = 0; i < numFree; i++) {
            solution[freeVars[i]] = (mask >> i) & 1;
        }

        // Back-substitute to find pivot variables
        for (int row = currentRow - 1; row >= 0; row--) {
            if (pivotCol[row] == -1) continue;

            int sum = matrix[row][numButtons];
            for (int col = pivotCol[row] + 1; col < numButtons; col++) {
                sum ^= (matrix[row][col] * solution[col]);
            }
            solution[pivotCol[row]] = sum;
        }

        // Count button presses
        int presses = 0;
        for (int val : solution) {
            presses += val;
        }
        minPresses = min(minPresses, presses);
    }

    return minPresses;
}

static string solvePart1(const string& input) {
    auto lines = util::splitLines(input);
    int totalPresses = 0;

    for (const auto& line : lines) {
        if (line.empty()) continue;
        Machine m = parseMachine(line);
        int presses = solveMinPresses(m);
        totalPresses += presses;
    }

    return to_string(totalPresses);
}

// Parse joltage requirements {x,y,z}
vector<int> parseJoltage(const string& line) {
    size_t start = line.find('{');
    size_t end = line.find('}');
    string joltageStr = line.substr(start + 1, end - start - 1);

    vector<int> joltage;
    stringstream ss(joltageStr);
    string num;
    while (getline(ss, num, ',')) {
        joltage.push_back(stoi(num));
    }
    return joltage;
}

// Hash function for vector<int>
struct VectorHash {
    size_t operator()(const vector<int>& v) const {
        size_t hash = 0;
        for (int val : v) {
            hash = hash * 100 + val;
        }
        return hash;
    }
};

// Hash function for parity vectors
struct ParityHash {
    size_t operator()(const vector<int>& v) const {
        size_t hash = 0;
        for (int val : v) {
            hash = hash * 2 + (val % 2);
        }
        return hash;
    }
};

// Build parity maps: for each parity pattern, find all reachable patterns with min button presses
unordered_map<vector<int>, unordered_map<vector<int>, int, VectorHash>, ParityHash>
buildParityMaps(int numCounters, const vector<vector<int>>& buttons) {
    unordered_map<vector<int>, unordered_map<vector<int>, int, VectorHash>, ParityHash> parityMaps;
    int numButtons = buttons.size();

    // Try all combinations of button presses (0 or 1 time each)
    for (int mask = 0; mask < (1 << numButtons); mask++) {
        vector<int> result(numCounters, 0);
        int numPresses = 0;

        for (int j = 0; j < numButtons; j++) {
            if (mask & (1 << j)) {
                for (int counter : buttons[j]) {
                    result[counter]++;
                }
                numPresses++;
            }
        }

        // Get parity pattern
        vector<int> parity(numCounters);
        for (int i = 0; i < numCounters; i++) {
            parity[i] = result[i] % 2;
        }

        // Update map with minimum presses for this pattern
        if (parityMaps[parity].find(result) == parityMaps[parity].end() ||
            parityMaps[parity][result] > numPresses) {
            parityMaps[parity][result] = numPresses;
        }
    }

    return parityMaps;
}

// Recursive function to find minimum button presses
pair<bool, int> findMinPresses(
    const vector<int>& current,
    const unordered_map<vector<int>, unordered_map<vector<int>, int, VectorHash>, ParityHash>& parityMaps,
    unordered_map<vector<int>, pair<bool, int>, VectorHash>& cache) {

    // Check cache
    if (cache.find(current) != cache.end()) {
        return cache[current];
    }

    // Base case: all zeros
    bool allZero = true;
    for (int val : current) {
        if (val != 0) {
            allZero = false;
            break;
        }
    }
    if (allZero) {
        cache[current] = {true, 0};
        return {true, 0};
    }

    // Check for negative values
    for (int val : current) {
        if (val < 0) {
            cache[current] = {false, 0};
            return {false, 0};
        }
    }

    // Get current parity
    vector<int> parity(current.size());
    for (size_t i = 0; i < current.size(); i++) {
        parity[i] = current[i] % 2;
    }

    // Check if this parity is achievable
    if (parityMaps.find(parity) == parityMaps.end()) {
        cache[current] = {false, 0};
        return {false, 0};
    }

    int minPresses = INT_MAX;

    // Try each pattern with matching parity
    for (const auto& [pattern, numPresses] : parityMaps.at(parity)) {
        // Check if pattern is valid (doesn't exceed current)
        bool valid = true;
        for (size_t i = 0; i < pattern.size(); i++) {
            if (pattern[i] > current[i]) {
                valid = false;
                break;
            }
        }
        if (!valid) continue;

        // Subtract pattern and divide by 2
        vector<int> next(current.size());
        for (size_t i = 0; i < current.size(); i++) {
            next[i] = (current[i] - pattern[i]) / 2;
        }

        // Recurse
        auto [reachedGoal, count] = findMinPresses(next, parityMaps, cache);
        if (reachedGoal) {
            minPresses = min(minPresses, numPresses + 2 * count);
        }
    }

    if (minPresses == INT_MAX) {
        cache[current] = {false, 0};
        return {false, 0};
    }

    cache[current] = {true, minPresses};
    return {true, minPresses};
}

int solveMinPressesJoltage(const vector<vector<int>>& buttons, const vector<int>& target) {
    int numCounters = target.size();

    // Build parity maps
    auto parityMaps = buildParityMaps(numCounters, buttons);

    // Find minimum presses with memoization
    unordered_map<vector<int>, pair<bool, int>, VectorHash> cache;
    auto [success, presses] = findMinPresses(target, parityMaps, cache);

    if (!success) {
        return INT_MAX;
    }

    return presses;
}

static string solvePart2(const string& input) {
    auto lines = util::splitLines(input);
    int totalPresses = 0;

    for (const auto& line : lines) {
        if (line.empty()) continue;

        Machine m = parseMachine(line);
        vector<int> joltage = parseJoltage(line);

        int presses = solveMinPressesJoltage(m.buttons, joltage);
        totalPresses += presses;
    }

    return to_string(totalPresses);
}

// ---- Auto-registration ----

struct AutoRegister2025Day10 {
    AutoRegister2025Day10() {
        registerSolution(2025, 10, solvePart1, solvePart2);
    }
};

static AutoRegister2025Day10 regInstance;
