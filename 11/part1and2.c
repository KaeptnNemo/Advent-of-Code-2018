#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define GRID_SIZE 300

typedef struct {
    int cells[GRID_SIZE][GRID_SIZE];
    int seriel_number;
} grid_t;

grid_t* new_grid(int seriel_number) {
    grid_t *g = malloc(sizeof(grid_t));
    g->seriel_number = seriel_number;

    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = 0; y < GRID_SIZE; y++) {
            int rack_id = x + 1 + 10;
            int power_level = rack_id * (y + 1) + seriel_number;
            power_level *= rack_id;
            power_level = (power_level % 1000) / 100;
            g->cells[x][y] = power_level - 5;
        }
    }
}

int best_square(grid_t *g, int square_size, int should_print) {
    int m, x, y;
    m = INT_MIN;
    for (int i = 0; i < GRID_SIZE-(square_size-1); i++) {
        for (int j = 0; j < GRID_SIZE-(square_size-1); j++) {
            int sum = 0;
            for (int a = 0; a < square_size; a++)
                for (int b = 0; b < square_size; b++)
                    sum += g->cells[i+a][j+b];
            if (sum > m) {
                x = i;
                y = j;
                m = sum;
            }
        }
    }

    if (should_print) printf("%i,%i,%i\n", x+1, y+1, square_size);
    else printf("%i\n", square_size);
    return m;
}

int main(int argc, char *argv[]) {
    grid_t *g = new_grid(atoi(argv[1]));

    /* part 1 */
    (void) best_square(g, 3, 1);

    /* part 2 */
    int m = INT_MIN;
    int s;
    for (int square_size = 1; square_size <= 300; square_size++) {
        int r = best_square(g, square_size, 0);
        if (r > m) { m = r; s = square_size; };
    }
    (void) best_square(g, s, 1);

    free(g);
    return 0;
}