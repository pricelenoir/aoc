#include <string>
#include <vector>
#include <sstream>
#include "../../util/registry.hpp"
#include "../../util/util.hpp"
using namespace std;

// ---- Day-specific solutions ----

struct Shape {
    int id;
    vector<string> display;
    int area;
};

struct Region {
    int width;
    int height;
    vector<int> count;
};

static string solvePart1(const string& input) {
    auto lines = util::splitLines(input);

    vector<Shape> shapes;
    vector<Region> regions;

    for (const auto& line : lines) {
        if (line.empty()) continue;

        // Shape definition (ends with ':')
        if (!line.empty() && line.back() == ':') {
            shapes.emplace_back();
            shapes.back().id = stoi(line.substr(0, line.size() - 1));
            shapes.back().area = 0;
        }
        // Shape display line (contains '.' or '#')
        else if (!line.empty() && (line[0] == '.' || line[0] == '#')) {
            shapes.back().display.push_back(line);
            // Count area
            for (char c : line) {
                if (c == '#') {
                    shapes.back().area++;
                }
            }
        }
        // Region definition (contains 'x' and ':')
        else if (line.find('x') != string::npos && line.find(':') != string::npos) {
            auto x_idx = line.find('x');
            auto colon_idx = line.find(':', x_idx + 1);

            regions.emplace_back();
            regions.back().width = stoi(line.substr(0, x_idx));
            regions.back().height = stoi(line.substr(x_idx + 1, colon_idx - x_idx - 1));

            // Parse counts
            string counts_str = line.substr(colon_idx + 1);
            istringstream iss(counts_str);
            int count;
            while (iss >> count) {
                regions.back().count.push_back(count);
            }
        }
    }

    // Count regions that can fit all presents
    int n_regions_fit_presents = 0;
    for (const auto& region : regions) {
        int total_used_area = 0;
        for (size_t idx = 0; idx < region.count.size(); idx++) {
            total_used_area += shapes[idx].area * region.count[idx];
        }

        int region_area = region.width * region.height;
        if (region_area >= total_used_area) {
            n_regions_fit_presents++;
        }
    }

    return to_string(n_regions_fit_presents);
}

static string solvePart2(const string& input) {
    (void)input;
    return "";
}

// ---- Auto-registration ----

struct AutoRegister2025Day12 {
    AutoRegister2025Day12() {
        registerSolution(2025, 12, solvePart1, solvePart2);
    }
};

static AutoRegister2025Day12 regInstance;
