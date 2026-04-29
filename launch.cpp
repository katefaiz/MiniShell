#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#ifdef __APPLE__
  #include <util.h>
#else
  #include <pty.h>
#endif
#include <sys/wait.h>
#include <assert.h>
#include <stdbool.h>

void Output(const char *buf, int len) {
    assert(buf);

    /* тут надо будет вставить логику работы нашего терминала, всякие приветствия, выводы команд и прочее */
    //fwrite(buf, 1, len, stdout);
    //fflush(stdout);
}

void Exit(void) {
    printf("\n[bash завершен]\n");
}

int main() {
    int master_fd = 0;

    pid_t pid = forkpty(&master_fd, NULL, NULL, NULL);

    if (pid == 0) {
        execl("/bin/bash", "bash", NULL);
        exit(1);

}
    char buf[256] = {};
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