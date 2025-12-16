#include <string>
#include "../../util/registry.hpp"
#include "../../util/util.hpp"
using namespace std;

// ---- Day-specific solutions ----

static string solvePart1(const string& input) {
    vector<string> inputVec = util::splitLines(input);

    int position = 50, direction = 0, rotation = 0, solution = 0;
    for (string s : inputVec) {
        direction = (s[0] == 'L') ? -1 : 1;
        rotation = stoi(s.substr(1));
        position += rotation * direction;
        position = ((position % 100) + 100) % 100;
        if (position == 0) solution++;
    }
    return to_string(solution);
}

static string solvePart2(const string& input) {
    vector<string> inputVec = util::splitLines(input);
    
    int position = 50, direction = 0, rotation = 0, solution = 0;
    for (string s : inputVec) {
        direction = (s[0] == 'L') ? -1 : 1;
        rotation = stoi(s.substr(1));
        
        // Brute force: move one step at a time
        for (int i = 0; i < rotation; i++) {
            position += direction;

            if (position >= 100) position -= 100;
            if (position < 0)    position += 100;
            if (position == 0)  solution++;
        }
    }
    return to_string(solution);
}

// ---- Auto-registration ----

struct AutoRegister2025Day1 {
    AutoRegister2025Day1() {
        registerSolution(2025, 1, solvePart1, solvePart2);
    }
};

static AutoRegister2025Day1 regInstance;
