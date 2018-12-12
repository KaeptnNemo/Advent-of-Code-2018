#include <stdlib.h>
#include <stdio.h>

typedef struct pot_t pot_t;
struct pot_t {
    int id;
    int current;
    int soon;
    pot_t *next;
    pot_t *prev;
};

typedef struct {
    pot_t *first;
    pot_t *last;
} chain_t;

pot_t* new_pot(int state) {
    pot_t *pot = malloc(sizeof(pot_t));
    pot->current = pot->soon = state;
    pot->next = pot->prev = NULL;
    return pot;
}

void delete_pot(pot_t *pot) {
    if (!pot) return;
    delete_pot(pot->next);
    free(pot);
}

void add_back(chain_t *chain, int state) {
    pot_t *pot = new_pot(state);

    if (chain->last) {
        chain->last->next = pot;
        pot->prev = chain->last;
        chain->last = pot;
        pot->id = pot->prev->id + 1;
    }
    else {
        pot->id = 0;
        chain->first = chain->last = pot;
    }
}

void add_front(chain_t *chain, int state) {
    pot_t *pot = new_pot(state);

    if (chain->first) {
        chain->first->prev = pot;
        pot->next = chain->first;
        chain->first = pot;
        pot->id = pot->next->id - 1;
    }
    else {
        pot->id = 0;
        chain->first = chain->last = pot;
    }
}

int peek(FILE *f) { return ungetc(fgetc(f),f); }

chain_t* new_chain(FILE *f) {
    chain_t *chain = malloc(sizeof(chain_t));
    int in = peek(f);
    while (in == '#' || in == '.') {
        add_back(chain, in == '#' ? 1 : 0);
        (void) fgetc(f); in = peek(f);
    }
    return chain;
}

void delete_chain(chain_t *chain) {
    delete_pot(chain->first);
    free(chain);
}

void print_chain(chain_t *chain, int i) {
    printf("% 3i | ", i);
    for (pot_t *iter = chain->first; iter; iter = iter->next)
        printf("%c", iter->current ? '#' : '.');
    printf("\n");
}

void grow_chain(chain_t *chain, int i) {
    while(i--) { add_back(chain, 0); add_front(chain, 0); }
}

void commit_generation(chain_t *chain) {
    for (pot_t *iter = chain->first; iter; iter = iter->next)
        iter->current = iter->soon;
}

int get_pattern_from_string(const char *cstr) {
    int bit0 = cstr[4] == '#' ? 1 : 0;
    int bit1 = cstr[3] == '#' ? 1 : 0;
    int bit2 = cstr[2] == '#' ? 1 : 0;
    int bit3 = cstr[1] == '#' ? 1 : 0;
    int bit4 = cstr[0] == '#' ? 1 : 0;
    return bit4 * 16 + bit3 * 8 + bit2 * 4 + bit1 * 2 + bit0;
}

int get_pattern_from_pot(pot_t *iter) {
    int bit0 = iter->next->next->current;
    int bit1 = iter->next->current;
    int bit2 = iter->current;
    int bit3 = iter->prev->current;
    int bit4 = iter->prev->prev->current;
    return bit4 * 16 + bit3 * 8 + bit2 * 4 + bit1 * 2 + bit0;
}

void do_generation(chain_t *chain, int *rules) {
    pot_t *end  = chain->last->prev;
    pot_t *iter = chain->first->next->next;

    while (iter != end) {
        int p = get_pattern_from_pot(iter);
        iter->soon = rules[p];
        iter = iter->next;
    }

    commit_generation(chain);
}

int sum_plants(chain_t *chain) {
    int sum = 0;
    for (pot_t *iter = chain->first; iter; iter = iter->next)
        sum += iter->current;
    return sum;
}

int sum_pot_ids(chain_t *chain) {
    int sum = 0;
    for (pot_t *iter = chain->first; iter; iter = iter->next)
        sum += iter->current * iter->id;
    return sum;
}

void skip_until_input(FILE *f) {
    int in;
    do { in = fgetc(f); } while (in != '#' && in != '.');
    ungetc(in, f);
}

void build_rules(FILE *f, int *rules) {
    char buf[6];
    for (int i = 0; i < 32; i++) {
        skip_until_input(f);
        fscanf(f, "%s", buf);
        int p = get_pattern_from_string(buf);
        skip_until_input(f);
        rules[p] = fgetc(f) == '#' ? 1 : 0;
    }
}

int main() {
    FILE *f = fopen("input.txt", "r");

    skip_until_input(f);
    chain_t *chain = new_chain(f);

    int rules[32];
    build_rules(f, rules);

    fclose(f);

    grow_chain(chain, 25);
    print_chain(chain, 0);
    for (int i = 0; i < 20; i++) {
        do_generation(chain, rules);
        print_chain(chain, i+1);
    }
    printf("there are %i plants\n", sum_plants(chain));
    printf("the sum of plant containing pot ids is %i\n", sum_pot_ids(chain));

    delete_chain(chain);
    return 0;
}