#include "command.h"

void AddVariable(const char* name, const char* value) {
    assert(name);
    assert(value);

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
    assert(name);

    for (int i = 0; i < variables_count; ++i) {
        if (strcmp(variables[i].name, name) == 0) 
            return variables[i].value;
    }

    return getenv(name);
}