#include <string>
#include "../../util/registry.hpp"
#include "../../util/util.hpp"
using namespace std;

// ---- Day-specific solutions ----

static string solvePart1(const string& input) {
    auto lines = util::splitLines(input);
    vector<pair<int, int>> tiles;

    // Parse all red tile coordinates
    for (const auto& line : lines) {
        if (line.empty()) continue;
        auto parts = util::split(line, ',');
        if (parts.size() == 2) {
            int x = stoi(parts[0]);
            int y = stoi(parts[1]);
            tiles.push_back({x, y});
        }
    }

    // Find the maximum rectangle area by checking all pairs of tiles
    long long maxArea = 0;
    for (size_t i = 0; i < tiles.size(); i++) {
        for (size_t j = i + 1; j < tiles.size(); j++) {
            long long x1 = tiles[i].first;
            long long y1 = tiles[i].second;
            long long x2 = tiles[j].first;
            long long y2 = tiles[j].second;

            // Add 1 to each dimension to include both corner tiles
            long long area = (abs(x2 - x1) + 1) * (abs(y2 - y1) + 1);
            maxArea = max(maxArea, area);
        }
    }
    return to_string(maxArea);
}

// Check if a point is inside or on the boundary of the polygon using ray casting
static bool isGreenTile(long long x, long long y, const vector<pair<int, int>>& polygon) {
    int n = polygon.size();

    // Check if on boundary (edges between consecutive points)
    for (int i = 0; i < n; i++) {
        int next = (i + 1) % n;
        long long x1 = polygon[i].first, y1 = polygon[i].second;
        long long x2 = polygon[next].first, y2 = polygon[next].second;

        // Check if point is on the line segment
        if (x1 == x2) { // Vertical line
            if (x == x1 && min(y1, y2) <= y && y <= max(y1, y2)) {
                return true;
            }
        } else if (y1 == y2) { // Horizontal line
            if (y == y1 && min(x1, x2) <= x && x <= max(x1, x2)) {
                return true;
            }
        }
    }

    // Ray casting algorithm for point in polygon
    bool inside = false;
    for (int i = 0; i < n; i++) {
        int next = (i + 1) % n;
        long long x1 = polygon[i].first, y1 = polygon[i].second;
        long long x2 = polygon[next].first, y2 = polygon[next].second;

        if ((y1 <= y && y < y2) || (y2 <= y && y < y1)) {
            // Calculate x coordinate of intersection
            double xIntersect = x1 + (double)(y - y1) / (y2 - y1) * (x2 - x1);
            if (x < xIntersect) {
                inside = !inside;
            }
        }
    }

    return inside;
}

static string solvePart2(const string& input) {
    auto lines = util::splitLines(input);
    vector<pair<int, int>> tiles;

    // Parse all red tile coordinates
    for (const auto& line : lines) {
        if (line.empty()) continue;
        auto parts = util::split(line, ',');
        if (parts.size() == 2) {
            int x = stoi(parts[0]);
            int y = stoi(parts[1]);
            tiles.push_back({x, y});
        }
    }

    // Find the maximum rectangle area where all tiles are red or green
    long long maxArea = 0;
    for (size_t i = 0; i < tiles.size(); i++) {
        for (size_t j = i + 1; j < tiles.size(); j++) {
            long long x1 = tiles[i].first;
            long long y1 = tiles[i].second;
            long long x2 = tiles[j].first;
            long long y2 = tiles[j].second;

            // Get rectangle bounds
            long long minX = min(x1, x2);
            long long maxX = max(x1, x2);
            long long minY = min(y1, y2);
            long long maxY = max(y1, y2);

            // Check if the rectangle is entirely within green/red area
            // Sample points along the edges to verify
            bool allValid = true;

            // Sample every ~1000 units to keep it fast but accurate
            long long step = max(1LL, (maxX - minX) / 100);

            // Check top and bottom edges
            for (long long x = minX; x <= maxX && allValid; x += step) {
                if (!isGreenTile(x, minY, tiles) || !isGreenTile(x, maxY, tiles)) {
                    allValid = false;
                }
            }
            // Check left and right edges
            for (long long y = minY; y <= maxY && allValid; y += step) {
                if (!isGreenTile(minX, y, tiles) || !isGreenTile(maxX, y, tiles)) {
                    allValid = false;
                }
            }
            // Always check corners
            if (allValid) {
                allValid = isGreenTile(minX, minY, tiles) &&
                          isGreenTile(minX, maxY, tiles) &&
                          isGreenTile(maxX, minY, tiles) &&
                          isGreenTile(maxX, maxY, tiles);
            }

            if (allValid) {
                long long area = (maxX - minX + 1) * (maxY - minY + 1);
                maxArea = max(maxArea, area);
            }
        }
    }

    return to_string(maxArea);
}

// ---- Auto-registration ----

struct AutoRegister2025Day9 {
    AutoRegister2025Day9() {
        registerSolution(2025, 9, solvePart1, solvePart2);
    }
};

static AutoRegister2025Day9 regInstance;
