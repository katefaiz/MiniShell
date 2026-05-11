#include "test.h"
#include "read.h"

int Test(char **tokens) {
    if (!tokens || !*tokens) {
        fprintf(stderr, RED("Error: tokens pointer is NULL.\n"));
        return NULL_ERROR;
    }
    
    int argc = 0;
    while (tokens[argc] != NULL) 
        argc++;

    char *saved_bracket = NULL;
    int bracket_idx = -1;

    if (strcmp(tokens[0], "[") == 0) {
        if (strcmp(tokens[argc - 1], "]") != 0) {
            fprintf(stderr, RED("[: missing `]'\n"));
            return 2;
        }

        bracket_idx = argc - 1;
        saved_bracket = tokens[bracket_idx];
        tokens[bracket_idx] = NULL;
    }

    int res = 1;

    if (tokens[1] == NULL) 
        res = 1;

    if (strcmp(tokens[1], "-f") == 0 && tokens[2]) {
        struct stat st;
        res = (stat(tokens[2], &st) == 0 && S_ISREG(st.st_mode)) ? 0 : 1;
    }
    else if (strcmp(tokens[1], "-d") == 0 && tokens[2]) {
        struct stat st;
        res = (stat(tokens[2], &st) == 0 && S_ISDIR(st.st_mode)) ? 0 : 1;
    }
    else if (strcmp(tokens[1], "-x") == 0 && tokens[2]) {
        res = (access(tokens[2], X_OK) == 0) ? 0 : 1;
    }    
    else if (strcmp(tokens[1], "-z") == 0 && tokens[2]) {
        if (strcmp(tokens[2], "\"\"") == 0 || strlen(tokens[2]) == 0)
            res = 0;
        res = 1;
    }
    else if (strcmp(tokens[1], "-n") == 0 && tokens[2]) {
        res = (strlen(tokens[2]) > 0) ? 0 : 1;
    }    
    else if (argc >= 4) {
        int a = atoi(tokens[1]);
        char *op = tokens[2];
        int b = atoi(tokens[3]);

        if (strcmp(op, "-eq") == 0)      res = (a == b) ? 0 : 1;
        else if (strcmp(op, "-ne") == 0) res = (a != b) ? 0 : 1;
        else if (strcmp(op, "-gt") == 0) res = (a > b) ? 0 : 1;
        else if (strcmp(op, "-ge") == 0) res = (a >= b) ? 0 : 1;
        else if (strcmp(op, "-lt") == 0) res = (a < b) ? 0 : 1;
        else if (strcmp(op, "-le") == 0) res = (a <= b) ? 0 : 1;
    }

    if (saved_bracket != NULL)
        tokens[bracket_idx] = saved_bracket;

    return res;
}