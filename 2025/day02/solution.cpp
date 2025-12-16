#include <string>
#include "../../util/registry.hpp"
#include "../../util/util.hpp"
#include <set>
using namespace std;

// ---- Day-specific solutions ----

long long ipow(int base, int exp) {
    long long result = 1;
    for (int i = 0; i < exp; i++) result *= base;
    return result;
}

string solvePart1(const string& input) {
    vector<string> inputVec = util::split(input, ',');
    long long total = 0;
    
    for (auto& range : inputVec) {
        if (range.empty()) continue;
        
        auto parts = util::split(range, '-');
        long long low = stoll(parts[0]);
        long long high = stoll(parts[1]);
        
        // For each possible half-length (1, 2, 3, ... digits)
        for (int halfLen = 1; halfLen <= 9; halfLen++) {
            long long multiplier = ipow(10, halfLen);
            
            // invalidId = base * multiplier + base = base * (multiplier + 1)
            // So: base = invalidId / (multiplier + 1)
            // 
            // Find the range of bases that produce invalid IDs in [low, high]
            long long factor = multiplier + 1;
            
            // Minimum base that could be in range
            long long baseMin = (low + factor - 1) / factor;  // ceiling division
            // Maximum base that could be in range
            long long baseMax = high / factor;
            
            // Also constrain to valid bases for this halfLen
            long long validBaseMin = (halfLen == 1) ? 1 : ipow(10, halfLen - 1);
            long long validBaseMax = ipow(10, halfLen) - 1;
            
            baseMin = max(baseMin, validBaseMin);
            baseMax = min(baseMax, validBaseMax);
            
            for (long long base = baseMin; base <= baseMax; base++) {
                long long invalidId = base * factor;
                total += invalidId;
            }
        }
    }
    
    return to_string(total);
}

string solvePart2(const string& input) {
    vector<string> inputVec = util::split(input, ',');
    long long total = 0;
    
    for (auto& range : inputVec) {
        if (range.empty()) continue;
        
        auto parts = util::split(range, '-');
        long long low = stoll(parts[0]);
        long long high = stoll(parts[1]);
        
        set<long long> invalidIds;
        
        // For each pattern length
        for (int patLen = 1; patLen <= 9; patLen++) {
            // For each number of repetitions (at least 2)
            for (int reps = 2; reps * patLen <= 18; reps++) {
                // Calculate factor: e.g., patLen=2, reps=3 -> 10101
                // base * factor gives us base repeated reps times
                long long factor = 0;
                for (int r = 0; r < reps; r++) {
                    factor += ipow(10, patLen * r);
                }
                
                long long baseMin = (low + factor - 1) / factor;
                long long baseMax = high / factor;
                
                long long validBaseMin = (patLen == 1) ? 1 : ipow(10, patLen - 1);
                long long validBaseMax = ipow(10, patLen) - 1;
                
                baseMin = max(baseMin, validBaseMin);
                baseMax = min(baseMax, validBaseMax);
                
                for (long long base = baseMin; base <= baseMax; base++) {
                    long long invalidId = base * factor;
                    invalidIds.insert(invalidId);
                }
            }
        }
        
        for (long long id : invalidIds) {
            total += id;
        }
    }
    
    return to_string(total);
}

// ---- Auto-registration ----
struct AutoRegister2025Day2 {
    AutoRegister2025Day2() {
        registerSolution(2025, 2, solvePart1, solvePart2);
    }
};
static AutoRegister2025Day2 regInstance;
