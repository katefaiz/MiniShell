#ifndef BUILTINS_H
#define BUILTINS_H

#include "tools.h"
#include "if_while.h"
#include "test.h"
#include "arithmetic.h"

static const char *builtins_commands[] = {"echo", "cd", "ls", "pwd", "exit", "type", "true", "false", "read"};

int EchoBuiltins(char **tokens);
int CdBuiltins(char **tokens);
int LsBuiltins(char **tokens);
int PwdBuiltins(char **tokens);
int ExitBuiltins(char **tokens);
int TypeBuiltins(char **tokens);
int TrueBuiltins(char **tokens);
int FalseBuiltins(char **tokens);
int ReadBuiltins(char **tokens);

void ReplaceVariables(char *buffer);
void ReplaceArithmeticSubstitutions(char *buffer);
int ExecuteCommands(char **tokens);

typedef struct {
    const char *cmd_name;
    int (*func)(char **tokens);
} CommandsPair;

static const CommandsPair commands_table[] = {
    {"echo", EchoBuiltins},
    {"cd", CdBuiltins},
    {"ls", LsBuiltins},
    {"pwd", PwdBuiltins},
    {"exit", ExitBuiltins},
    {"type", TypeBuiltins},
    {"true", TrueBuiltins},
    {"false", FalseBuiltins},
    {"read", ReadBuiltins},
    {"if", If},
    {"elif", If},
    {"while", While},
    {"[", Test},
    {"test", Test},
    {"((", ComparisonsArithmetic}, 
    {NULL, NULL}
};

static const int MAX_CNT_VARIABLES = 256;

typedef struct {
    char* name;
    char* value;
} OneVariable;

static OneVariable variables[MAX_CNT_VARIABLES];
static int variables_count = 0;

#endif //BUILTINS