#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include "../../util/registry.hpp"
#include "../../util/util.hpp"
using namespace std;

// ---- Day-specific solutions ----

// Union-Find (Disjoint Set Union) data structure
// Efficiently tracks which junction boxes belong to which circuit
class UnionFind {
private:
    vector<int> parent;  // parent[i] = parent of node i
    vector<int> size;    // size[i] = size of the tree rooted at i

public:
    UnionFind(int n) : parent(n), size(n, 1) {
        // Initially, each node is its own parent (separate circuit)
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    // Find the root/representative of the set containing x
    // Uses path compression for efficiency
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);  // Path compression
        }
        return parent[x];
    }

    // Union two sets containing x and y
    // Returns true if they were in different sets (successful merge)
    // Returns false if they were already in the same set
    bool unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);

        if (rootX == rootY) {
            return false;  // Already in same circuit
        }

        // Union by size: attach smaller tree under larger tree
        if (size[rootX] < size[rootY]) {
            parent[rootX] = rootY;
            size[rootY] += size[rootX];
        } else {
            parent[rootY] = rootX;
            size[rootX] += size[rootY];
        }

        return true;  // Successfully merged
    }

    // Get the size of the circuit containing node x
    int getSize(int x) {
        return size[find(x)];
    }
};

// Structure to represent a 3D point
struct Point {
    int x, y, z;
};

// Structure to represent an edge between two junction boxes
struct Edge {
    int from, to;
    double distance;

    // For sorting edges by distance
    bool operator<(const Edge& other) const {
        return distance < other.distance;
    }
};

// Calculate Euclidean distance between two 3D points
double calcDistance(const Point& a, const Point& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    double dz = a.z - b.z;
    return sqrt(dx*dx + dy*dy + dz*dz);
}

static string solvePart1(const string& input) {
    // Parse input: read all junction box coordinates
    vector<Point> junctionBoxes;
    auto lines = util::splitLines(input);

    for (const auto& line : lines) {
        if (line.empty()) continue;
        auto coords = util::split(line, ',');
        Point p;
        p.x = stoi(coords[0]);
        p.y = stoi(coords[1]);
        p.z = stoi(coords[2]);
        junctionBoxes.push_back(p);
    }

    int n = junctionBoxes.size();

    // Generate all possible edges (pairs of junction boxes) with their distances
    vector<Edge> edges;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            Edge e;
            e.from = i;
            e.to = j;
            e.distance = calcDistance(junctionBoxes[i], junctionBoxes[j]);
            edges.push_back(e);
        }
    }

    // Sort edges by distance (shortest first)
    // This is key to the greedy algorithm - we always connect the closest boxes
    sort(edges.begin(), edges.end());

    // Initialize Union-Find to track circuits
    UnionFind uf(n);

    // Attempt to connect the closest pairs of junction boxes
    // Some attempts may fail if the boxes are already in the same circuit
    // For the example: 20 boxes, 10 attempts
    // For the actual input: many boxes, 1000 attempts
    int pairsToAttempt = (n == 20) ? 10 : 1000;
    int pairsAttempted = 0;

    for (const auto& edge : edges) {
        if (pairsAttempted >= pairsToAttempt) {
            break;
        }

        // Try to connect these two junction boxes
        // If they're already in the same circuit, unite returns false but we still count the attempt
        uf.unite(edge.from, edge.to);
        pairsAttempted++;
    }

    // Find all unique circuits and their sizes
    vector<int> circuitSizes;
    vector<bool> counted(n, false);

    for (int i = 0; i < n; i++) {
        int root = uf.find(i);
        if (!counted[root]) {
            circuitSizes.push_back(uf.getSize(i));
            counted[root] = true;
        }
    }

    // Sort circuit sizes to find the three largest
    sort(circuitSizes.begin(), circuitSizes.end(), greater<int>());

    // Multiply the three largest circuit sizes
    long long result = (long long)circuitSizes[0] * circuitSizes[1] * circuitSizes[2];
    return to_string(result);
}

static string solvePart2(const string& input) {
    // Parse input: read all junction box coordinates
    vector<Point> junctionBoxes;
    auto lines = util::splitLines(input);

    for (const auto& line : lines) {
        if (line.empty()) continue;
        auto coords = util::split(line, ',');
        Point p;
        p.x = stoi(coords[0]);
        p.y = stoi(coords[1]);
        p.z = stoi(coords[2]);
        junctionBoxes.push_back(p);
    }

    int n = junctionBoxes.size();

    // Generate all possible edges (pairs of junction boxes) with their distances
    vector<Edge> edges;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            Edge e;
            e.from = i;
            e.to = j;
            e.distance = calcDistance(junctionBoxes[i], junctionBoxes[j]);
            edges.push_back(e);
        }
    }

    // Sort edges by distance (shortest first)
    sort(edges.begin(), edges.end());

    // Initialize Union-Find to track circuits
    UnionFind uf(n);

    // Keep connecting until all boxes are in one circuit
    // Track the last connection that actually merged two circuits
    int lastFrom = -1, lastTo = -1;
    int circuitsRemaining = n;  // Start with n separate circuits

    for (const auto& edge : edges) {
        // Try to connect these two junction boxes
        if (uf.unite(edge.from, edge.to)) {
            // Successfully merged two circuits
            circuitsRemaining--;
            lastFrom = edge.from;
            lastTo = edge.to;

            // If we're down to 1 circuit, we're done
            if (circuitsRemaining == 1) {
                break;
            }
        }
    }

    // Multiply the X coordinates of the last two junction boxes connected
    long long result = (long long)junctionBoxes[lastFrom].x * junctionBoxes[lastTo].x;
    return to_string(result);
}

// ---- Auto-registration ----

struct AutoRegister2025Day8 {
    AutoRegister2025Day8() {
        registerSolution(2025, 8, solvePart1, solvePart2);
    }
};

static AutoRegister2025Day8 regInstance;
