#include <cstdio>
#include <vector>
#include <algorithm>
#include <climits>

struct vec {
    long x, y;
    vec() {}
    vec(long x, long y) : x(x), y(y) {}
};

struct point {
    vec old, now, vel;
    point() {}
    point(long x, long y, long dx, long dy) { now = vec(x, y); vel = vec(dx, dy); }

    void update() {
        old = now;
        now.x += vel.x;
        now.y += vel.y;
    }
};

void get_min_max(std::vector<point> &points, vec &min, vec &max) {
    auto minmax_x = std::minmax_element(points.begin(), points.end(),
        [](point p1, point p2) { return p1.now.x < p2.now.x; });
    auto minmax_y = std::minmax_element(points.begin(), points.end(),
        [](point p1, point p2) { return p1.now.y < p2.now.y; });
    min.x = minmax_x.first->now.x;
    max.x = minmax_x.second->now.x;
    min.y = minmax_y.first->now.y;
    max.y = minmax_y.second->now.y;
}

struct bounding_area {
    vec min, max;
    long area, area_old;
    int seconds;

    long compute_width()  { return max.x - min.x + 1; }
    long compute_height() { return max.y - min.y + 1; }
    long compute_area()   { return compute_width() * compute_height(); }

    bounding_area(std::vector<point> &points) {
        seconds = 0;
        get_min_max(points, min, max);
        area = compute_area();
    }

    bool update(std::vector<point> &points) {
        seconds++;
        for (auto &p : points) p.update();
        get_min_max(points, min, max);
        area_old = area;
        area = compute_area();
        return compute_height() > 10;
    }
};

bool parse_point(FILE *f, point &p) {
    long x, y, dx, dy;
    if (fscanf(f, "position=<%li, %li> " ,  &x,  &y) != 2) return false;
    if (fscanf(f, "velocity=<%li, %li>\n", &dx, &dy) != 2) return false;
    p = point(x, y, dx, dy);
    return true;
}

int main() {

    /* all points */
    std::vector<point> points;

    /* parse input */
    {
        FILE *in = fopen("input.txt", "r");
        point p;
        while (parse_point(in, p)) points.push_back(p);
        fclose(in);
    }

    bounding_area b(points);
    while (b.update(points));

    char *msg = new char[b.compute_area()];
    std::fill(msg, msg + b.compute_area(), ' ');
    for (auto &p : points) {
        p.now.x -= b.min.x;
        p.now.y -= b.min.y;
        msg[p.now.y*b.compute_width()+p.now.x] = '#';
    }
    for (long y = 0; y < b.compute_height(); y++) {
        for (long x = 0; x < b.compute_width(); x++) {
            printf("%c", msg[y*b.compute_width()+x]);
        }
        printf("\n");
    }

    printf("simulated %i seconds\n", b.seconds);

    delete[] msg;
    return 0;
}