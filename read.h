#ifndef READ_H_
#define READ_H_

#include "tools.h"

const int REALLOC_COEFF = 2;
const int START_CAPACITY = 128;


enum status {
    SUCCESS               = 0,
    OPEN_ERROR            = 1,
    STAT_ERROR            = 2,
    NOT_ENOUGH_MEMORY     = 3,
    READ_ERROR            = 4,
    CLOSE_ERROR           = 5
};


struct about_text {
    char* buffer = NULL;
    int cnt_words = 0;
    size_t size = 0;
    size_t capacity = 0;
    char** pointers_on_words = NULL;
};


status Read(about_text *text, FILE* file);

status Fragmentation(about_text *text);

void TextDtor(about_text* text);


#endif //READ_H_