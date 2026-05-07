#ifdef __APPLE__ 
  #include <util.h>
#else
  #include <pty.h>
#endif
#include <termios.h>

#include "tools.h"
#include "subsidiary.h"

struct termios orig_termios;

void InitSHLVL(void) {
    char *shlvl = getenv("SHLVL");
    int level = shlvl ? atoi(shlvl) + 1 : 1;

    INIT_BUFFER(buf, DEFAULT_SMALL_BUF_SIZE);
    snprintf(buf, sizeof(buf), "%d", level);
    setenv("SHLVL", buf, 1);
}

void EnableRawMode(void) {
    tcgetattr(STDIN_FILENO, &orig_termios);
    struct termios raw = orig_termios;

    cfmakeraw(&raw);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

void DisableRawMode(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

void Output(const char *buf, int len) {
    if (!buf) {
        fprintf(stderr, RED("Error: buf NULL pointer.\n"));
        return;
    }

    fwrite(buf, 1, len, stdout);
    fflush(stdout);
}

int main(void) {
    InitSHLVL();

    if (!getenv("PWD")) {
        INIT_BUFFER(cwd, PATH_MAX);
        getcwd(cwd, sizeof(cwd));
        setenv("PWD", cwd, 1);
    }

    EnableRawMode();
    int master_fd = 0;
    pid_t pid = forkpty(&master_fd, NULL, NULL, NULL);
    if (pid == 0) {
        execl("./minishell", "minishell", NULL);
        exit(1);
    }

    INIT_BUFFER(buf, DEFAULT_MIDDLE_BUF_SIZE);
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
    DisableRawMode();
    PrintExit();
    return 0;
}