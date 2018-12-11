#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>

constexpr int THRESHOLD = 10000;

struct pos {
    int id; int x; int y;

    pos() {}
    pos(int x, int y) : x(x), y(y) {}

    static int distance(pos p1, pos p2) {
        return std::abs(p1.x-p2.x) + std::abs(p1.y-p2.y);
    }
};

int main() {
    std::vector<pos> positions;

    /* parse input */
    FILE *in = fopen("input.txt", "r");
    int id = 0;
    while (true) {
        pos p; p.id = id++;
        if (fscanf(in, "%i, %i\n", &p.x, &p.y) != 2) break;
        positions.push_back(p);
    }
    fclose(in);

    /* get bounds */
    int xmin, xmax, ymin, ymax;
    {
        auto xminmax = std::minmax_element(positions.begin(), positions.end(),
            [](pos p1, pos p2) { return p1.x < p2.x; } );
        auto yminmax = std::minmax_element(positions.begin(), positions.end(),
            [](pos p1, pos p2) { return p1.y < p2.y; } );
        xmin = xminmax.first->x;
        xmax = xminmax.second->x;
        ymin = yminmax.first->y;
        ymax = yminmax.second->y;
    }

    /* align positions to (0,0) */
    for (auto &p : positions) { p.x -= xmin; p.y -= ymin; }
    xmax -= xmin; ymax -= ymin;
    int width  = xmax + 1;
    int height = ymax + 1;

    /* for each field - for each position: check if distance < THRESHOLD and add it to area */
    int area_size = 0;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < width; y++) {
            pos f(x,y);
            int total_dist = 0;
            for (auto p : positions) total_dist += pos::distance(p, f);
            if (total_dist < THRESHOLD) area_size++;
        }
    }

    printf("area size: %i\n", area_size);

    return 0;
}