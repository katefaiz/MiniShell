#include "arithmetic.h"

static void SkipSpaces(const char **s) {
    while (**s && isspace(**s)) (*s)++;
}

//считывает число, перемещая указатель
static int GetNumber(const char **s, int *val) {
    SkipSpaces(s);
    char *end;
    *val = (int)strtol(*s, &end, 10);

    if (*s == end) return 1;   
    *s = end;

    return 0;
}

int EvaluateArithmetic(const char* expr) {
    if (!expr) return 0;

    const char *s = expr;
    int invert = 0;

    SkipSpaces(&s);
    if (*s == '!') {
        invert = 1;
        s++;
        SkipSpaces(&s);
    }

    if (*s != '(' || *(s+1) != '(') {
        fprintf(stderr, "arithmetic: expected (( ... ))\n");
        return 0;
    }

    s += 2;
    SkipSpaces(&s);

    int left, right = 0;
    // Левый
    if (GetNumber(&s, &left) == 1) {
        fprintf(stderr, "arithmetic: left operand is not a number\n");
        return 0;
    }
    SkipSpaces(&s);

    char op[4] = {0};
    if (strncmp(s, "<=", 2) == 0) { 
        strcpy(op, "<="); 
        s += 2; 
    }
    else if (strncmp(s, ">=", 2) == 0) { 
        strcpy(op, ">="); 
        s += 2; 
    }
    else if (strncmp(s, "==", 2) == 0) { 
        strcpy(op, "=="); 
        s += 2; 
    }
    else if (strncmp(s, "!=", 2) == 0) { 
        strcpy(op, "!="); 
        s += 2; 
    }
    else if (*s == '<') { 
        op[0] = '<'; 
        s += 1; 
    }
    else if (*s == '>') { 
        op[0] = '>'; 
        s += 1; 
    }
    else {
        fprintf(stderr, "arithmetic: unknown operator\n");
        return 0;
    }
    SkipSpaces(&s);

    // Правый 
    if (GetNumber(&s, &right) == 1) {
        fprintf(stderr, "arithmetic: right operand is not a number\n");
        return 0;
    }
    SkipSpaces(&s);

    if (*s != ')' || *(s+1) != ')') {
        fprintf(stderr, "arithmetic: expected ))\n");
        return 0;
    }

    int result = 0;
    if      (strcmp(op, "==") == 0) result = (left == right);
    else if (strcmp(op, "!=") == 0) result = (left != right);
    else if (strcmp(op, "<") == 0)  result = (left < right);
    else if (strcmp(op, ">") == 0)  result = (left > right);
    else if (strcmp(op, "<=") == 0) result = (left <= right);
    else if (strcmp(op, ">=") == 0) result = (left >= right);
    else {
        fprintf(stderr, "arithmetic: unknown operator %s\n", op);
        result = 0;
    }

    if (invert) result = !result;

    return result;
}

// int main() {
//     char *line = NULL;
//     size_t len = 0;
//     ssize_t nread;


//     while (1) {
//         printf("> ");
//         fflush(stdout);

//         nread = getline(&line, &len, stdin);
//         if (nread == -1) {  
//             printf("\n");
//             break;
//         }

//         if (nread > 0 && line[nread - 1] == '\n')
//             line[nread - 1] = '\0';

//         if (strlen(line) == 0)
//             continue;

//         int result = EvaluateArithmetic(line);
//         printf("%d\n", result);
//     }

//     free(line);
//     return 0;
// }