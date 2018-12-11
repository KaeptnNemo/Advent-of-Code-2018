#include <cstdio>
#include <vector>
#include <algorithm>


int main() {

    /* has_dependencies[c-'A'] -> number of dependencies left */
    int has_dependencies[26];
    std::fill(has_dependencies, has_dependencies + 26, 0);

    /* c must be done before must_be_done_before[c-'A'] */
    std::vector<int> must_be_done_before[26];

    /* worker i is working on task working_on[i] */
    int working_on[5];
    std::fill(working_on, working_on + 5, -1);

    /* worker i has time_left[i] seconds left until the task is done */
    int time_left[5];
    std::fill(time_left, time_left + 5, 1);

    /* parse input */
    FILE *in = fopen("input.txt", "r");
    while (true) {
        char d, t; // dependency, target
        if(fscanf(in, "Step %c must be finished before step %c can begin.\n", &d, &t) != 2) break;
        must_be_done_before[d-'A'].push_back(t-'A');
        has_dependencies[t-'A']++;
    }
    fclose(in);


    int todo = 26;
    int second = 0;
    std::vector<int> ready_ones;
    while (todo) {

        /* update times */
        for (int i = 0; i < 5; i++) if (time_left[i]) time_left[i]--;

        /* update dependencies due to completed tasks */
        for (int i = 0; i < 5; i++) {
            /* for each free worker */
            if (!time_left[i]) {

                /* if worked on something, then remove dependencies */
                if (working_on[i] != -1) {
                    for (int t : must_be_done_before[working_on[i]]) has_dependencies[t]--;
                    printf("%c", working_on[i] + 'A');
                    working_on[i] = -1;
                    todo--;
                }
            }
        }

        /* assign new work */
        for (int i = 0; i < 5; i++) {
            /* for each free worker */
            if (!time_left[i]) {

                /* find a tasks and assign */
                ready_ones.clear();
                for (int t = 0; t < 26; t++)
                    if (has_dependencies[t] == 0)
                        ready_ones.push_back(t);
                if (ready_ones.size()) {
                    int next = *std::min_element(ready_ones.begin(), ready_ones.end());
                    has_dependencies[next] = -1;
                    working_on[i] = next;
                    time_left[i] = 61 + next;
                }
            }
        }

        /* update time */
        second++;
    }

    printf("\ntime: %i\n", second-1);
    return 0;
}