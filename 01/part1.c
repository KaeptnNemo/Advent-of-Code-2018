#include <stdio.h>

int getSign(FILE *f) {
    int c;
    do {
        c = fgetc(f);
    } while (c != '+' && c != '-' && c != EOF);

    switch (c) {
        case EOF: return  0;
        case '+': return  1;
        case '-': return -1;
    }
}

int parseNumber(FILE *f) {
    int sign = getSign(f);
    int i = 0;
    fscanf(f, "%i", &i);
    return i * sign;
}

int peek(FILE *f) {
    return ungetc(fgetc(f), f);
}

int addNumbers(FILE *f) {
    int sum = 0;
    int c = peek(f);
    while (c != EOF) {
        sum += parseNumber(f);
        c = peek(f);
    }
    return sum;
}

int main(int argc, char *argv[]) {
    FILE *f = fopen(argv[1], "r");
    int result = addNumbers(f);
    fclose(f);

    printf("%i\n", result);
    return 0;
}