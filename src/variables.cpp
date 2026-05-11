#include "command.h"

void AddVariable(const char* name, const char* value) {
    if (!name) {
        fprintf(stderr, RED("Error: name pointer is NULL.\n"));
        return;
    }
    if (!value) {
        fprintf(stderr, RED("Error: value pointer is NULL.\n"));
        return;
    }

    for (int i = 0; i < variables_count; ++i) {
        if (strcmp(variables[i].name, name) == 0) {
            free(variables[i].value);
            variables[i].value = strdup(value);

            return;
        }
    }

    if (variables_count < MAX_CNT_VARIABLES) {
        variables[variables_count].name = strdup(name);
        variables[variables_count].value = strdup(value);
        variables_count++;
    }
}

const char* GetVariable(const char* name) {
    if (!name) {
        fprintf(stderr, RED("Error: name pointer is NULL.\n"));
        return NULL;
    }

    for (int i = 0; i < variables_count; ++i) {
        if (strcmp(variables[i].name, name) == 0) 
            return variables[i].value;
    }

    return getenv(name);
}