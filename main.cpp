#include <stdio.h>

#include "arithmetic.h"
#include "command.h"
#include "read.h"
#include "tools.h"

int main() {
    about_text text;
    status s;

    while (1) {
        char buffer[PATH_MAX];
        getcwd(buffer, sizeof(buffer)); 
        printf(BLUE("%s :) "), buffer); 

        fflush(stdout);

        s = Read(&text, stdin);
        if (s == READ_ERROR) { 
            printf("\n");
            break;
        }
        if (s != SUCCESS) {
            fprintf(stderr, "Read error\n");
            break;
        }

        ReplaceArithmeticSubstitutions(text.buffer);
        s = Fragmentation(&text);
        if (s != SUCCESS) {
            fprintf(stderr, "Fragmentation error\n");
            TextDtor(&text);
            continue;
        }

        if (text.cnt_words > 0) {
            ExecuteCommands(text.pointers_on_words);
        }

        TextDtor(&text);
    }

    return 0;
}