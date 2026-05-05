#ifndef BUILTINS_H
#define BUILTINS_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <sys/stat.h>


const char *builtins_commands[] = {"echo", "cd", "ls", "pwd", "exit", "type", "true", "false", "read"};

int EchoBuiltins(char **tokens);
int CdBuiltins(char **tokens);
int LsBuiltins(char **tokens);
int PwdBuiltins(char **tokens);
int ExitBuiltins(char **tokens);
int TypeBuiltins(char **tokens);
int TrueBuiltins(char **tokens);
int FalseBuiltins(char **tokens);
int ReadBuiltins(char **tokens);

int ExecuteBuiltin(char **tokens);

typedef struct {
    const char *cmd_name;
    int (*func)(char **tokens);
} BuiltinPair;

BuiltinPair builtins_table[] = {
    {"echo", EchoBuiltins},
    {"cd", CdBuiltins},
    {"ls", LsBuiltins},
    {"pwd", PwdBuiltins},
    {"exit", ExitBuiltins},
    {"type", TypeBuiltins},
    {"true", TrueBuiltins},
    {"false", FalseBuiltins},
    {"read", ReadBuiltins},
    {NULL, NULL}
};
#endif //BUILTINS