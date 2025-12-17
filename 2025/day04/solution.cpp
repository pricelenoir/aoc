#include <pthread.h>
#include <string>
#include "../../util/registry.hpp"
#include "../../util/util.hpp"
#include <queue>
using namespace std;

// ---- Day-specific solutions ----

static string solvePart1(const string& input) {
    vector<string> grid = util::splitLines(input);
    int solution = 0;

    int directions[8][2] = {{1,0}, {1, 1}, {1, -1}, {-1,0}, {-1,1}, {-1,-1}, {0,1}, {0,-1}};
    
    int X = (int)grid.size();
    int Y = (int)grid[0].size();
    for (int i = 0; i < X; i++) {
        for (int j = 0; j < Y; j++) {
            if (grid[i][j] != '@') continue;
            int count = 0;
            for (int k = 0; k < 8; k++) {
                int nextX = i + directions[k][0];
                int nextY = j + directions[k][1];
                if (nextX >= 0 && nextX < X && nextY >= 0 && nextY < Y) {
                    if (grid[nextX][nextY] == '@') count++;
                }
            }
            if (count < 4) solution++;
            count = 0;
        }
    }
    return to_string(solution);
}

static string solvePart2(const string& input) {
    vector<string> grid = util::splitLines(input);
    int solution = 0;
    int directions[8][2] = {{1,0},{1,1},{1,-1},{-1,0},{-1,1},{-1,-1},{0,1},{0,-1}};
    int X = (int)grid.size();
    int Y = (int)grid[0].size();

    auto countNeighbors = [&](int i, int j) {
        int count = 0;
        for (int k = 0; k < 8; k++) {
            int ni = i + directions[k][0];
            int nj = j + directions[k][1];
            if (ni >= 0 && ni < X && nj >= 0 && nj < Y && grid[ni][nj] == '@') {
                count++;
            }
        }
        return count;
    };

    // BFS approach: only the neighbors of cells that can be reached would then be reachable thereafter
    // Basically boils down into a multi-source BFS traversal
    queue<pair<int,int>> q;

    // Add all initial reachable cells
    for (int i = 0; i < X; i++) {
        for (int j = 0; j < Y; j++) {
            if (grid[i][j] == '@' && countNeighbors(i, j) < 4) {
                q.push({i,j});
            }
        }
    }

    // BFS
    while (!q.empty()) {
        auto [i,j] = q.front();
        q.pop();

        if (grid[i][j] != '@') continue;
        if (countNeighbors(i, j) >= 4) continue;

        grid[i][j] = '.';
        solution++;

        // Check if any neighbors became accessible
        for (int k = 0; k < 8; k++) {
            int ni = i + directions[k][0];
            int nj = j + directions[k][1];
            if (ni >= 0 && ni < X && nj >= 0 && nj < Y && grid[ni][nj] == '@') {
                q.push({ni,nj});
            }
        }
    }
    return to_string(solution);
}

// ---- Auto-registration ----

struct AutoRegister2025Day4 {
    AutoRegister2025Day4() {
        registerSolution(2025, 4, solvePart1, solvePart2);
    }
};

static AutoRegister2025Day4 regInstance;
