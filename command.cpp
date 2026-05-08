#include "read.h"
#include "command.h"

int EchoBuiltins(char **tokens) {
    if (!tokens || !*tokens) {
        fprintf(stderr, RED("Error: tokens pointer is NULL.\n"));
        return NULL_ERROR;
    }
    
    for (int i = 1; tokens[i] != NULL; ++i) {
        printf("%s", tokens[i]);
        if (tokens[i+1] != NULL)
            printf(" ");
    }
    printf("\n");

    return 0;
}

int CdBuiltins(char **tokens) {
    if (!tokens || !*tokens) {
        fprintf(stderr, RED("Error: tokens pointer is NULL.\n"));
        return NULL_ERROR;
    }

    const char* path = tokens[1];
    if (path == NULL) {
        path = getenv("HOME");
    }

    char *old = getenv("PWD");
    if (old) setenv("OLDPWD", old, 1);

    if (chdir(path) != 0) {
        fprintf(stderr, RED("Error: no such file or directory\n"));
        return 1;
    }

    char cwd[PATH_MAX] = {};
    getcwd(cwd, sizeof(cwd));
    setenv("PWD", cwd, 1);
    return 0;
}

int LsBuiltins(char **tokens) {
    if (!tokens || !*tokens) {
        fprintf(stderr, RED("Error: tokens pointer is NULL.\n"));
        return NULL_ERROR;
    }

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
    if (!tokens || !*tokens) {
        fprintf(stderr, RED("Error: tokens pointer is NULL.\n"));
        return NULL_ERROR;
    }

    char buffer[PATH_MAX];
    getcwd(buffer, sizeof(buffer)); 
    printf("%s\n", buffer);
    return 0;

}

int ExitBuiltins(char **tokens) {
    if (!tokens || !*tokens) {
        fprintf(stderr, RED("Error: tokens pointer is NULL.\n"));
        return NULL_ERROR;
    }
    exit(0);
}

int TypeBuiltins(char **tokens) {
    if (!tokens || !*tokens) {
        fprintf(stderr, RED("Error: tokens pointer is NULL.\n"));
        return NULL_ERROR;
    }

    if (tokens[1] == NULL) {
        fprintf(stderr, "Error type: missing argument.\n");
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
        fprintf(stderr, RED("%s not found\n"), tokens[1]);
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
        fprintf(stderr, RED("%s not found\n"), tokens[1]);
    return found ? 0 : 1;
}

int TrueBuiltins(char **tokens) {
    if (!tokens || !*tokens) {
        fprintf(stderr, RED("Error: tokens pointer is NULL.\n"));
        return NULL_ERROR;
    }
    return 0;
}

int FalseBuiltins(char **tokens) {
    if (!tokens || !*tokens) {
        fprintf(stderr, RED("Error: tokens pointer is NULL.\n"));
        return NULL_ERROR;
    }
    return 1;
}

int ReadBuiltins(char **tokens) {
    if (!tokens || !*tokens) {
        fprintf(stderr, RED("Error: tokens pointer is NULL.\n"));
        return NULL_ERROR;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t nread = getline(&line, &len, stdin);
    if (nread > 0 && line[nread-1] == '\n') line[nread-1] = '\0';

    printf("%s\n", line);
    free(line);

    return 0;
}

//*----------------------------------------------------------------
void ReplaceVariables(char *buffer) {
    if (!buffer) {
        fprintf(stderr, RED("Error: nothing is in a buffer.\n"));
        return;
    }

    size_t buf_size = strlen(buffer) + DEFAULT_BIG_BUF_SIZE;
    char *new_buf = (char *) calloc (buf_size, 1);
    if (!new_buf) {
        fprintf(stderr, RED("Error: memory allocation failed.\n"));
        return;
    }

    char *out = new_buf;
    char *in = buffer;

    while (*in) {
        if (*in == '$' && *(in + 1) != '(' && *(in + 1) != '\0') {
            in++;
            INIT_BUFFER(var_name, DEFAULT_MIDDLE_BUF_SIZE);
            int j = 0;

            while (*in && (isalnum((unsigned char)*in) || *in == '_')) {
                var_name[j++] = *in++;
            }
            var_name[j] = '\0';

            char *val = getenv(var_name);
            if (val) {
                strcpy(out, val);
                out += strlen(val);
            }
            continue;
        }

        *out++ = *in++;
    }

    *out = '\0';
    strcpy(buffer, new_buf);
    free(new_buf);
}

void ReplaceArithmeticSubstitutions(char *buffer) {
    if (!buffer) {
        fprintf(stderr, RED("Error: nothing is in a buffer.\n"));
        return;
    }

    size_t buf_size = strlen(buffer) + 1024;
    char *new_buf = (char*)calloc(buf_size, 1);
    if (!new_buf) {
        fprintf(stderr, RED("Error: memory allocation failed.\n"));
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
    if (!tokens || !*tokens) {
        fprintf(stderr, RED("Error: tokens pointer is NULL.\n"));
        return NULL_ERROR;
    }

    if (strcmp(tokens[0], "!") == 0) {
        if (tokens[1] == NULL) {
            printf("!: missing argument\n");
            return 1;
        }
        int res = ExecuteCommands(tokens + 1);
        return (res == 0) ? 1 : 0;
    }

    for (int i = 0; commands_table[i].cmd_name != NULL; i++) {
        if (strcmp(tokens[0], commands_table[i].cmd_name) == 0) {
            return commands_table[i].func(tokens);
        }
    }

    pid_t pid = fork(); // тут проверка вызова файла (из разряда ./ququ)
    if (pid == 0) {
        if (execvp(tokens[0], tokens) == -1) {
            fprintf(stderr, RED("Error: command not found: %s :((\n"), tokens[0]);
            exit(127);
        }

    } else if (pid < 0) {
        perror("fork");
        return 1;

    } else {
        int status = 0;
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        }
    }

    fprintf(stderr, RED("Error: command not found: %s :((\n"), tokens[0]);
    return 1;
}

