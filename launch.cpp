#ifdef __APPLE__ 
  #include <util.h>
#else
  #include <pty.h>
#endif

#include "tools.h"

#define DEFAULT_BIG_BUF_SIZE    1024
#define DEFAULT_MIDDLE_BUF_SIZE 256
#define DEFAULT_SMALL_BUF_SIZE  16

void InitSHLVL(void) {
    char *shlvl = getenv("SHLVL");
    int level = shlvl ? atoi(shlvl) + 1 : 1;

    char buf[DEFAULT_SMALL_BUF_SIZE] = {};
    snprintf(buf, sizeof(buf), "%d", level);
    setenv("SHLVL", buf, 1);
}

void ShellCD(const char *path) {
    assert(path); // TODO: красивый if  с выводом некорректного пути

    char *old = getenv("PWD");
    if (old) {
        setenv("OLDPWD", old, 1);

        if (chdir(path) == 0) {
            char cwd[DEFAULT_BIG_BUF_SIZE] = {};
            getcwd(cwd, sizeof(cwd));
            setenv("PWD", cwd, 1);
        } else {
            perror("cd"); // TODO: аналогично красивый вывод ошибки
        }
    }

}
void InitEnv(void) {
    InitSHLVL();
    
    if (!getenv("PWD")) {
        char cwd[DEFAULT_BIG_BUF_SIZE] = {};
        getcwd(cwd, sizeof(cwd));
        setenv("PWD", cwd, 1);
    }
}

void Output(const char *buf, int len) {
    assert(buf);

    // тут надо будет вставить логику работы нашего терминала, всякие приветствия, выводы команд и прочее
    fwrite(buf, 1, len, stdout);
    fflush(stdout);
}

void Exit(void) {
    printf(GREEN("\n[shell завершен]\n"));
}

int main() {
    InitEnv();
    int master_fd = 0;

    pid_t pid = forkpty(&master_fd, NULL, NULL, NULL);

    if (pid == 0) {
        execl("./minishell", "bash", NULL); // TODO: тут потом заменю путь
        exit(1);

}
    char buf[DEFAULT_MIDDLE_BUF_SIZE] = {};
    ssize_t n = 0;

    while (true) {
        fd_set fds = {};
        FD_ZERO(&fds);
        FD_SET(0, &fds);
        FD_SET(master_fd, &fds);

        if (select(master_fd + 1, &fds, NULL, NULL, NULL) < 0) break;

        if (FD_ISSET(0, &fds)) {
            n = read(0, buf, sizeof(buf));
            if (n <= 0) break;
            write(master_fd, buf, n);
        }

        if (FD_ISSET(master_fd, &fds)) {
            n = read(master_fd, buf, sizeof(buf) - 1);
            if (n <= 0) break;
            buf[n] = '\0';
            Output(buf, n);
        }
    }

    waitpid(pid, NULL, 0);
    Exit();
    return 0;
}