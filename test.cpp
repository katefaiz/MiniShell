#include "test.h"

int Test(char **tokens) {
    int argc = 0;
    while (tokens[argc] != NULL) 
        argc++;

    if (strcmp(tokens[0], "[") == 0) {
        if (strcmp(tokens[argc - 1], "]") != 0) {
            fprintf(stderr, "[: missing `]'\n");
            return 2;
        }

        tokens[argc - 1] = NULL;
    }

    if (tokens[1] == NULL) 
        return 1;

    if (strcmp(tokens[1], "-f") == 0 && tokens[2]) {
        struct stat st;
        return (stat(tokens[2], &st) == 0 && S_ISREG(st.st_mode)) ? 0 : 1;
    }
    if (strcmp(tokens[1], "-d") == 0 && tokens[2]) {
        struct stat st;
        return (stat(tokens[2], &st) == 0 && S_ISDIR(st.st_mode)) ? 0 : 1;
    }
    if (strcmp(tokens[1], "-x") == 0 && tokens[2]) {
        return (access(tokens[2], X_OK) == 0) ? 0 : 1;
    }

    if (strcmp(tokens[1], "-z") == 0 && tokens[2]) {
        if (strcmp(tokens[2], "\"\"") == 0 || strlen(tokens[2]) == 0) {
            return 0;
        }
        return (strlen(tokens[2]) == 0) ? 0 : 1;
    }
    if (strcmp(tokens[1], "-n") == 0 && tokens[2]) {
        return (strlen(tokens[2]) > 0) ? 0 : 1;
    }

    if (argc >= 4) {
        int a = atoi(tokens[1]);
        char *op = tokens[2];
        int b = atoi(tokens[3]);

        if (strcmp(op, "-eq") == 0) return (a == b) ? 0 : 1;
        if (strcmp(op, "-ne") == 0) return (a != b) ? 0 : 1;
        if (strcmp(op, "-gt") == 0) return (a > b) ? 0 : 1;
        if (strcmp(op, "-ge") == 0) return (a >= b) ? 0 : 1;
        if (strcmp(op, "-lt") == 0) return (a < b) ? 0 : 1;
        if (strcmp(op, "-le") == 0) return (a <= b) ? 0 : 1;
    }

    return 1;
}