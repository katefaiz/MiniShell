#include "read.h"

static void SkipSpaces(char** str) {
    assert(str != NULL);
    assert(*str != NULL);

    while (**str != '\0' && isspace(**str))
        (*str)++;
}

static void SkipWord(char** str) {
    assert(str != NULL);
    assert(*str != NULL);

    while (**str != '\0' && !isspace(**str))
        (*str)++;
}

static char* ResizeBuffer(char* buffer, size_t* capacity) {
    assert(buffer);

    *capacity *= REALLOC_COEFF;
    char* new_buf = (char*)realloc(buffer, *capacity);
    if (new_buf == NULL) {
        free(buffer);

        return NULL;
    }

    return new_buf;
}

status Read(about_text *text, FILE* file) {
    assert(text != NULL);
    assert(file != NULL);

    text->size = 0;
    text->capacity = START_CAPACITY;
    text->buffer = (char*)calloc(text->capacity, sizeof(char));
    if (text->buffer == NULL) 
        return NOT_ENOUGH_MEMORY;

    int c = 0;
    while ((c = fgetc(file)) != EOF) {
        if (c == '\\') {
            int next = fgetc(file);
            if (next == '\n')
                continue; 
            else
                ungetc(next, file);
        }

        if (c == '\n') 
            break;

        if (text->size + 1 >= text->capacity) {
            text->buffer = ResizeBuffer(text->buffer, &text->capacity);
            if (text->buffer == NULL) 
                return NOT_ENOUGH_MEMORY;
        }

        text->buffer[text->size++] = (char)c;
    }

    if (text->size == 0 && c == EOF) {
        free(text->buffer);

        return READ_ERROR;
    }

    text->buffer[text->size] = '\0';

    return SUCCESS;
}

status Fragmentation(about_text *text) {
    assert(text != NULL);
    assert(text->buffer != NULL);

    text->cnt_words = 0;
    char* ptr = text->buffer;
    while (*ptr != '\0') {
        SkipSpaces(&ptr);
        
        if (*ptr != '\0') {
            text->cnt_words++;
            SkipWord(&ptr);
        }
    }

    if (text->cnt_words == 0) {
        text->pointers_on_words = NULL;

        return SUCCESS;
    }

    text->pointers_on_words = (char**)calloc(text->cnt_words + 1, sizeof(char*));
    if (text->pointers_on_words == NULL) 
        return NOT_ENOUGH_MEMORY;

    ptr = text->buffer;
    for (int i = 0; i < text->cnt_words; ++i) {
        SkipSpaces(&ptr);
        
        text->pointers_on_words[i] = ptr;
        SkipWord(&ptr);
        
        if (*ptr != '\0')
            *(ptr++) = '\0';
    }
    text->pointers_on_words[text->cnt_words] = NULL;

    return SUCCESS;
}

void TextDtor(about_text* text) {
    if (text == NULL) 
        return;

    free(text->buffer);
    free(text->pointers_on_words);
}
