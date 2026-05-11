#include "read.h"

static void SkipSpaces(char** str) {
    if (!str || !*str) {
        fprintf(stderr, RED("Error: str NULL poiner.\n"));
        return;
    }

    while (**str != '\0' && isspace(**str))
        (*str)++;
}

static void SkipWord(char** str) {
    if (!str || !*str) {
        fprintf(stderr, RED("Error: str NULL poiner.\n"));
        return;
    }

    while (**str != '\0' && !isspace(**str))
        (*str)++;
}

static char* ResizeBuffer(char* buffer, size_t* capacity) {
    if (!buffer) {
        fprintf(stderr, RED("Error: buffer NULL pointer.\n"));
        return NULL;
    }
    if (!capacity) {
        fprintf(stderr, RED("Error: capacity NULL pointer.\n"));
        return NULL;
    }

    *capacity *= REALLOC_COEFF;
    char* new_buf = (char*)realloc(buffer, *capacity);
    if (new_buf == NULL) {
        free(buffer);

        return NULL;
    }

    return new_buf;
}

status Read(about_text *text, FILE* file) {
    if (!text) {
        fprintf(stderr, RED("Error: text NULL pointer.\n"));
        return NULL_ERROR;
    }
    if (!file) {
        fprintf(stderr, RED("Error: file NULL pointer.\n"));
        return NULL_ERROR;
    }

    text->size = 0;
    text->capacity = START_CAPACITY;
    text->buffer = (char*)calloc(text->capacity, sizeof(char));
    if (text->buffer == NULL) 
        return NOT_ENOUGH_MEMORY;

    int c = 0;
    while ((c = fgetc(file)) != EOF) {
        if (c == '\\') {
            int next = fgetc(file);
            if (next == '\n') {
                text->buffer[text->size++] = ' ';
                text->buffer[text->size++] = ';';
                text->buffer[text->size++] = ' ';
                continue;
            } else {
                ungetc(next, file);
            }
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
    if (!text) {
        fprintf(stderr, RED("Error: text NULL pointer.\n"));
        return NULL_ERROR;
    }
    if (!text->buffer) {
        fprintf(stderr, RED("Error: text->buffer NULL pointer.\n"));
        return NULL_ERROR;
    }

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
