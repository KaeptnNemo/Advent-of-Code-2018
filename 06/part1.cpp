#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>

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

    /* init area */
    int* area = new int[width * height];
    std::fill(area, area + width * height, -1);
    for (auto p : positions) area[p.y*width+p.x] = p.id;

    /* for each pos - for each field: check if field belongs to pos and mark it if so */
    for (auto p : positions) {
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                if (area[y*width+x] != -1) continue; /* skip if marked */
                pos f(x,y);
                int dist = pos::distance(p, f);
                bool mine = true;
                for (auto other : positions) {
                    if (other.id == p.id) continue;
                    int other_dist = pos::distance(other, f);
                    if (other_dist <= dist) { mine = false; break; }
                }
                if (mine) area[y*width+x] = p.id;
            }
        } 
    }

    /* mark positions that have an infinite area */
    bool *is_at_border = new bool[positions.size()];
    std::fill(is_at_border, is_at_border + positions.size(), false);
    for (int x = 0; x < width; x++) {
        int id;
        id = area[0*width+x];
        if (id != -1) is_at_border[id] = true;
        id = area[ymax*width+x];
        if (id != -1) is_at_border[id] = true;
    }
    for (int y = 0; y < height; y++) {
        int id;
        id = area[y*width+0];
        if (id != -1) is_at_border[id] = true;
        id = area[y*width+xmax];
        if (id != -1) is_at_border[id] = true;
    }

    /* compute area sizes */
    int *area_size = new int[positions.size()];
    std::fill(area_size, area_size + positions.size(), 0);
    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++)
            if (area[y*width+x] != -1) area_size[area[y*width+x]]++;

    /* find id with max area */
    auto max_elem = std::max_element(positions.begin(), positions.end(),
        [&](pos p1, pos p2) {
            if (is_at_border[p1.id]) return true;
            if (is_at_border[p2.id]) return false;
            return area_size[p1.id] < area_size[p2.id];
        } );
    int max_id = max_elem->id;

    printf("max area: #%i with %i\n", max_id, area_size[max_id]);

    delete[] area;
    delete[] is_at_border;
    delete[] area_size;

    return 0;
}