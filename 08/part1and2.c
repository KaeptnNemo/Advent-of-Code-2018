#include <stdio.h>
#include <stdlib.h>

typedef struct node_t node_t;
struct node_t {
    int child_count;
    int meta_count;
    node_t *children;
    int *meta;
};

int get_number(FILE *f) {
    int i;
    fscanf(f, "%i", &i);
    return i;
}

void parse_node(node_t *self, FILE *f) {
    self->child_count = get_number(f);
    self->meta_count  = get_number(f);
    self->children    = malloc(sizeof(node_t) * self->child_count);
    self->meta        = malloc(sizeof(int) * self->meta_count);

    for (int i = 0; i < self->child_count; i++)
        parse_node(self->children + i, f);

    for (int i = 0; i < self->meta_count; i++)
        self->meta[i] = get_number(f);
}

void destroy_node(node_t *self) {
    for (int i = 0; i < self->child_count; i++)
        destroy_node(self->children + i);
    free(self->children);
    free(self->meta);
}

int add_meta(node_t *self) {
    int sum = 0;
    for (int i = 0; i < self->child_count; i++)
        sum += add_meta(self->children + i);
    for (int i = 0; i < self->meta_count; i++)
        sum += self->meta[i];
    return sum;
}

int value_of_node(node_t *self) {
    if (self->child_count == 0) return add_meta(self);

    int sum = 0;
    for (int i = 0; i < self->meta_count; i++) {
        int meta = self->meta[i] - 1;
        if (meta < 0 || meta >= self->child_count)
            continue;
        sum += value_of_node(self->children + meta);
    }
    return sum;
}

int main() {
    FILE *in = fopen("input.txt", "r");

    node_t root;
    parse_node(&root, in);

    fclose(in);

    int meta_sum = add_meta(&root);
    int value    = value_of_node(&root);
    destroy_node(&root);

    printf("part1: %i\n", meta_sum);
    printf("part2: %i\n", value);
    return 0;
}