#include "if_while.h"
#include "command.h"
#include "read.h"

static int FindToken(char** tokens, const char* target, int start) {
    if (!tokens || !*tokens) {
        fprintf(stderr, RED("Error: tokens pointer is NULL.\n"));
        return NULL_ERROR;
    }
    if (!target) {
        fprintf(stderr, RED("Error: target NULL pointer.\n"));
        return NULL_ERROR;
    }

    for (int i = start; tokens[i] != NULL; ++i) {
        if (strcmp(tokens[i], target) == 0) 
            return i;
    }

    return -1;
}

int If(char** tokens) {
    if (!tokens || !*tokens) {
        fprintf(stderr, RED("Error: tokens pointer is NULL.\n"));
        return NULL_ERROR;
    } 

    int then_idx = FindToken(tokens, "then", 1);
    int fi_idx   = FindToken(tokens, "fi", 1);
    int else_idx = FindToken(tokens, "else", 1);
    int elif_idx = FindToken(tokens, "elif", 1);

    if (then_idx == -1 || fi_idx == -1) {
        fprintf(stderr, RED("Syntax error: missing then/fi.\n"));
        return 1;
    }

    tokens[then_idx] = NULL;
    int cond_res = ExecuteCommands(&tokens[1]); 
    tokens[then_idx] = (char*)"then";

    if (cond_res == 0) {
        int end_of_block = fi_idx;

        if (elif_idx != -1) end_of_block = elif_idx;
        if (else_idx != -1 && else_idx < end_of_block) end_of_block = else_idx;

        char *saved = tokens[end_of_block];
        tokens[end_of_block] = NULL;
        ExecuteCommands(&tokens[then_idx + 1]);
        tokens[end_of_block] = saved;
        return 0; 
    } 
    else if (elif_idx != -1) {
        return If(&tokens[elif_idx]);
    }
    else if (else_idx != -1) {
        tokens[fi_idx] = NULL;
        ExecuteCommands(&tokens[else_idx + 1]);
        tokens[fi_idx] = (char*)"fi";
    }

    return 0;
}

int While(char** tokens) {
    if (!tokens || !*tokens) {
        fprintf(stderr, RED("Error: tokens pointer is NULL.\n"));
        return NULL_ERROR;
    }

    int do_idx = -1, done_idx = -1;
    
    for (int i = 0; tokens[i] != NULL; i++) {
        if (strcmp(tokens[i], "do") == 0 && do_idx == -1) do_idx = i;
        if (strcmp(tokens[i], "done") == 0)               done_idx = i;
    }

    if (do_idx == -1 || done_idx == -1) {
        fprintf(stderr, RED("Syntax error: missing do/done\n"));
        return 1;
    }

    while (1) {
        tokens[do_idx] = NULL;
        int cond = ExecuteCommands(tokens + 1);
        tokens[do_idx] = (char*)"do";

        if (cond != 0) break;

        tokens[done_idx] = NULL;
        ExecuteCommands(tokens + do_idx + 1);
        tokens[done_idx] = (char*)"done";
    }

    return 0;
}