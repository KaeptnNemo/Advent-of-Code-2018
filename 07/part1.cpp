#include <cstdio>
#include <vector>
#include <algorithm>


int main() {

    /* has_dependencies[c-'A'] -> number of dependencies left */
    int has_dependencies[26];
    std::fill(has_dependencies, has_dependencies + 26, 0);

    /* c must be done before must_be_done_before[c-'A'] */
    std::vector<int> must_be_done_before[26];

    /* parse input */
    FILE *in = fopen("input.txt", "r");
    while (true) {
        char d, t; // dependency, target
        if(fscanf(in, "Step %c must be finished before step %c can begin.\n", &d, &t) != 2) break;
        must_be_done_before[d-'A'].push_back(t-'A');
        has_dependencies[t-'A']++;
    }
    fclose(in);

    /* get next task and print */
    std::vector<int> ready_ones;
    int todo = 26;
    while (todo--) {
        for (int i = 0; i < 26; i++)
            if (has_dependencies[i] == 0) ready_ones.push_back(i);
        int next = *std::min_element(ready_ones.begin(), ready_ones.end());
        has_dependencies[next] = -1;
        ready_ones.clear();
        for (int i : must_be_done_before[next]) has_dependencies[i]--;
        printf("%c", next+'A');
    }

    printf("\n");
    return 0;
}