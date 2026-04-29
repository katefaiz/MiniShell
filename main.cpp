#include <stdio.h>

#include "read.h"


int main() {

    while (true) {
        about_text text = {};
        Read(&text, stdin);
        Fragmentation(&text);

        for (int i = 0; i < text.cnt_words; ++i)
            fprintf(stdout, "'%s'\n", text.pointers_on_words[i]);
    }

    return 0;
}