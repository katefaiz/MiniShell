#include "arithmetic.h"
#include "read.h"
#include "tools.h"

static void SkipSpaces(const char **s) {
    if (!s || !*s) {
        fprintf(stderr, RED("Error: s NULL pointer.\n"));
        return;
    }
    while (**s && isspace(**s)) (*s)++;
}

static int ParseNumber(const char **s, int *val) {
    if (!s || !*s) {
        fprintf(stderr, RED("Error: s NULL pointer.\n"));
        return NULL_ERROR;
    }
    if (!val) {
        fprintf(stderr, RED("Error: val NULL pointer.\n"));
        return NULL_ERROR;
    }
    SkipSpaces(s);
    char *end;
    *val = (int)strtol(*s, &end, 10);
    if (*s == end) return 0; //ничего не считано
    *s = end;

    return 1;
}

static int EvaluateComparison(const char *expr) {
    if (!expr) return 0;
    const char *s = expr;
    int invert = 0;

    SkipSpaces(&s);
    if (*s == '!') {
        invert = 1;
        s++;
        SkipSpaces(&s);
    }

    int left, right;
    if (!ParseNumber(&s, &left)) {
        printf("arithmetic: left operand is not a number\n");
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
        s ++; 
    }
    else if (*s == '>') { 
        op[0] = '>'; 
        s ++; 
    }
    else {
        printf("arithmetic: unknown operator\n");
        return 0;
    }
    SkipSpaces(&s);

    if (!ParseNumber(&s, &right)) {
        printf("arithmetic: right operand is not a number\n");
        return 0;
    }
    SkipSpaces(&s);
    if (*s != '\0') {
        printf("arithmetic: extra characters after expression\n");
        return 0;
    }

    int result = 0;
    if      (strcmp(op, "==") == 0) result = (left == right);
    else if (strcmp(op, "!=") == 0) result = (left != right);
    else if (strcmp(op, "<") == 0)  result = (left < right);
    else if (strcmp(op, ">") == 0)  result = (left > right);
    else if (strcmp(op, "<=") == 0) result = (left <= right);
    else if (strcmp(op, ">=") == 0) result = (left >= right);
    else result = 0;

    return invert ? !result : result;
}


int ComparisonsArithmetic(char **tokens) {
    if (!tokens || !*tokens) {
        fprintf(stderr, RED("Error: tokens pointer is NULL.\n"));
        return NULL_ERROR;
    }
    //if (tokens[0] == NULL) return 1;
    int last = 0;
    while (tokens[last] != NULL) last++;
    last--; //индекс последнего токена

    if (strncmp(tokens[0], "((", 2) != 0) {
        printf("ComparisonsArithmetic: expected (( ... ))\n");
        return 1;
    }
    if (last < 1 || (strstr(tokens[last], "))") == NULL && strcmp(tokens[last], "))") != 0)) {
        printf("ComparisonsArithmetic: missing closing ))\n");
        return 1;
    }

    size_t expr_len = 0;
    for (int i = 1; i < last; i++) {
        expr_len += strlen(tokens[i]) + 1; 
    }
    char *expr = (char*)malloc(expr_len + 1);
    if (!expr) {
        fprintf(stderr, "memory allocation error\n");
        return 1;
    }
    expr[0] = '\0';
    for (int i = 1; i < last; i++) {
        strcat(expr, tokens[i]);
        if (i < last - 1) strcat(expr, " ");
    }
    int res = EvaluateComparison(expr);
    free(expr);

    return res ? 0 : 1; 
}

//*--------------------------------------------------------------

char* EvaluateArithmeticExpression(const char *expr) {
    if (!expr) {
        fprintf(stderr, RED("Error: expr pointer is NULL.\n"));
        return NULL;
    }
    const char *s = expr;
    int left, right;
    if (!ParseNumber(&s, &left)) {
        printf("arithmetic: invalid left operand in %s\n", expr);
        return strdup("0");
    }
    SkipSpaces(&s);
    if (!*s) {
        printf("arithmetic: missing operator in %s\n", expr);
        return strdup("0");
    }
    char op = *s;
    if (op != '+' && op != '-' && op != '*' && op != '/' && op != '%') {
        printf("arithmetic: unknown operator '%c' in %s\n", op, expr);
        return strdup("0");
    }
    s++;
    if (!ParseNumber(&s, &right)) {
        printf("arithmetic: invalid right operand in %s\n", expr);
        return strdup("0");
    }
    SkipSpaces(&s);
    if (*s != '\0') {
        printf("arithmetic: extra characters in %s\n", expr);
        return strdup("0");
    }

    int result = 0;
    switch (op) {
        case '+': 
            result = left + right; 
            break;
        case '-': 
            result = left - right; 
            break;
        case '*': 
            result = left * right; 
            break;
        case '/': 
            result = left / right;
            break;
        case '%': 
            result = left % right; 
            break;
        default: return strdup("0");
    }
    char *res_str = (char*)malloc(16);
    if (!res_str) return strdup("0");
    snprintf(res_str, 16, "%d", result);
    return res_str;
}


