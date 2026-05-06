#include "if_while.h"
#include "command.h"

static int FindToken(char **tokens, const char *target, int start) {
    for (int i = start; tokens[i] != NULL; ++i) {
        if (strcmp(tokens[i], target) == 0) 
            return i;
    }

    return -1;
}

int If(char **tokens) {
    int then_idx = FindToken(tokens, "then", 1);
    int fi_idx = FindToken(tokens, "fi", 1);
    int else_idx = FindToken(tokens, "else", 1);

    if (then_idx == -1 || fi_idx == -1) {
        fprintf(stderr, "syntax error: missing then/fi\n");
        return 1;
    }

    tokens[then_idx] = NULL;
    
    int cond_res = ExecuteCommands(&tokens[1]); 
    
    tokens[then_idx] = (char*)"then";

    if (cond_res == 0) {
        int end_block = (else_idx != -1) ? else_idx : fi_idx;
        char *saved_token = tokens[end_block];
        
        tokens[end_block] = NULL;
        ExecuteCommands(&tokens[then_idx + 1]);
        tokens[end_block] = saved_token; 
    } 
    else if (else_idx != -1) {
        tokens[fi_idx] = NULL;
        ExecuteCommands(&tokens[else_idx + 1]);
        tokens[fi_idx] = (char*)"fi";
    }

    return 0;
}

int While(char **tokens) {
    int do_idx = FindToken(tokens, "do", 1);
    int done_idx = FindToken(tokens, "done", 1);

    if (do_idx == -1 || done_idx == -1) {
        fprintf(stderr, "syntax error: missing do/done\n");
        return 1;
    }

    while (1) {
        tokens[do_idx] = NULL;
        int cond_res = ExecuteCommands(&tokens[1]);
        tokens[do_idx] = (char*)"do";

        if (cond_res != 0) break; 

        tokens[done_idx] = NULL;
        ExecuteCommands(&tokens[do_idx + 1]);
        tokens[done_idx] = (char*)"done";
    }

    return 0;
}