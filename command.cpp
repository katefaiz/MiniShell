#include "read.h"
#include "command.h"

int EchoBuiltins(char **tokens) {
    for (int i = 1; tokens[i] != NULL; ++i) {
        printf("%s", tokens[i]);
        if (tokens[i+1] != NULL)
            printf(" ");
    }
    printf("\n");

    return 0;
}

int CdBuiltins(char **tokens) {
    const char* path = tokens[1];
    if (path == NULL) {
        path = getenv("HOME");
    }
    if (chdir(path) != 0) {
        printf(RED("No such file or directory\n"));
        return 1;
    }
    return 0;
}

int LsBuiltins(char **tokens) {
    const char *dir_path = tokens[1] ? tokens[1] : ".";
    DIR *dir = opendir(dir_path);
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.')
            continue;

        printf("%s\n", entry->d_name);
    }
    closedir(dir);
    return 0;
}

int PwdBuiltins(char **tokens) {
    char buffer[PATH_MAX];
    getcwd(buffer, sizeof(buffer)); 
    printf("%s\n", buffer);
    return 0;

}

int ExitBuiltins(char **tokens) {
    exit(0);
}

int TypeBuiltins(char **tokens) {
    if (tokens[1] == NULL) {
        fprintf(stderr, "type: missing argument\n");
        return 1;
    }
    
    for (int i = 0; commands_table[i].cmd_name != NULL; i++) {
        if (strcmp(tokens[1], commands_table[i].cmd_name) == 0) {
            printf("%s is a shell builtin\n", tokens[1]);
            return 0;
        }
    }
    //ищем в PATH
    char* path_env = getenv("PATH");
    char* path_copy = strdup(path_env); 
    if (!path_copy) {
        printf("%s not found\n", tokens[1]);
        return 1;
    }
    char* dir = strtok(path_copy, ":");
    int found = 0;
    while (dir != NULL) {
        char full_path[PATH_MAX];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, tokens[1]); //путь/имя
        if (access(full_path, X_OK) == 0) { //проверяет, существует ли файл по пути full_path и имеет ли пользователь право на его выполнение (X_OK — execute)
            printf("%s is %s\n", tokens[1], full_path);
            found = 1;
            break;
        }
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    if (!found)
        printf("%s not found\n", tokens[1]);
    return found ? 0 : 1;
}

int TrueBuiltins(char **tokens) {
    return 0;
}

int FalseBuiltins(char **tokens) {
    return 1;
}

int ReadBuiltins(char **tokens) {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread = getline(&line, &len, stdin);
    if (nread > 0 && line[nread-1] == '\n') line[nread-1] = '\0';

    printf("%s\n", line);
    free(line);

    return 0;
}

//*----------------------------------------------------------------

void ReplaceArithmeticSubstitutions(char *buffer) {
    size_t buf_size = strlen(buffer) + 1024;
    char *new_buf = (char*)calloc(buf_size, 1);
    if (!new_buf) {
        fprintf(stderr, "memory allocation error\n");
    }
    char *out = new_buf;
    char *in = buffer;

    while (*in) {
        if (strncmp(in, "$((", 3) == 0) {
            char *close = strstr(in + 3, "))");
            if (close) {

                size_t expr_len = close - (in + 3);
                char *expr = (char*)malloc(expr_len + 1);
                strncpy(expr, in + 3, expr_len);
                expr[expr_len] = '\0';

                char *res = EvaluateArithmeticExpression(expr);
                strcpy(out, res);
                out += strlen(res);

                free(res);
                free(expr);
                in = close + 2;

                continue;
            }
        }
        *out++ = *in++;
    }
    *out = '\0';
    strcpy(buffer, new_buf);
    free(new_buf);
}

int ExecuteCommands(char **tokens) {
    if (tokens == NULL || tokens[0] == NULL)
        return 0;
    if (strcmp(tokens[0], "!") == 0) {
        if (tokens[1] == NULL) {
            printf("!: missing argument\n");
            return 1;
        }
        int res = ExecuteCommands(tokens + 1);
        return (res == 0) ? 1 : 0;
    }
    for (int i = 0; commands_table[i].cmd_name != NULL; i++) {
        if (strcmp(tokens[0], commands_table[i].cmd_name) == 0)
            return commands_table[i].func(tokens);
    }
    printf("command not found: %s\n", tokens[0]);
    return 1;
}

