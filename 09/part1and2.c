#include <stdlib.h>
#include <stdio.h>

typedef struct marble_t marble_t;
struct marble_t {
    int id;
    marble_t *next;
    marble_t *prev;
};

marble_t* new_marble(int id) {
    marble_t *m = malloc(sizeof(marble_t));
    m->id = id;
    m->next = m->prev = m;
    return m;
}

marble_t* move_clockwise(marble_t *m, int times) {
    if (times > 0) while (times--) m = m->next;
    else           while (times++) m = m->prev;
    return m;
}

void insert_after(marble_t *m, marble_t *after) {
    m->prev = after;
    m->next = after->next;
    after->next = m;
    m->next->prev = m;
}

marble_t* remove_at(marble_t *m) {
    marble_t *right = m->next;
    m->prev->next = m->next;
    m->next->prev = m->prev;
    free(m);
    return right;
}

void remove_all(marble_t *m) {
    while (m->next != m) m = remove_at(m);
    free(m);
}

int main(int argc, char *argv[]) {
    int  player_count   = atoi(argv[1]);

    /* remve '* 100' for part 1 */
    long highest_marble = atoi(argv[2]) * 100;
    long *scores = calloc(sizeof(long), player_count);

    int last_placed = 0;
    int turn = 0;
    marble_t *current = new_marble(0);

    while (last_placed != highest_marble) {
        int next = last_placed + 1;

        if (next && !(next % 23)) {
            scores[turn] += next;
            current = move_clockwise(current, -7);
            scores[turn] += current->id;
            current = remove_at(current);
        }
        else {
            current = move_clockwise(current, 1);
            insert_after(new_marble(next), current);
            current = move_clockwise(current, 1);
        }

        last_placed = next;
        turn = (turn + 1) % player_count;
    }

    remove_all(current);

    long highest_score = 0;
    for (int i = 0; i < player_count; i++)
        if (scores[i] > highest_score) highest_score = scores[i];

    printf("%li\n", highest_score);
    return 0;
}

