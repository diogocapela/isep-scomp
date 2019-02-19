#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    const int NR_OF_CHILDREN = 4;
    pid_t pids[NR_OF_CHILDREN];

    int i;
    for (i = 0; i < NR_OF_CHILDREN; i++) {
        pids[i] = fork();

        if (pids[i] < 0) {
            perror("Fork failed!");
            exit(-1);
        }

        if (pids[i] == 0) {
            int timeToWait = i + 1;

            printf("Filho #%d criado com o PID = %d (tempo de espera de %d segundos)\n", i, getpid(), timeToWait);

            sleep(timeToWait);
            exit(timeToWait);
        }
    }

    pid_t p;
    int status;
    for (i = 0; i < NR_OF_CHILDREN; i++) {
        p = waitpid(pids[i], &status, 0);
        if (WIFEXITED(status)) {
            printf("O filho #%d com o PID = %d terminou com valor = %d\n", i, p, WEXITSTATUS(status));
        }
    }

    return 0;
}
