#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main() {
    // Initialization, should only be called once.
    

    /* random int between 0 and 19 */
    // int r = rand() % 20;

    const int LEITURA = 0;
    const int ESCRITA = 1;

    const int CONTINUE = 1;
    const int TERMINATE = 0;

    const int STARTING_CREDIT = 20;

    const int WIN_CREDIT = 10;
    const int LOSE_CREDIT = -5;

    const int BUFFER_SIZE = sizeof(int);

    // File descriptor (0 stdin, 1 stdout, 2 stderr)
    int fdUp[2];
    int fdDown[2];

    // Criar o pipe (0 em sucesso, -1 em erro) (estrutura FIFO)
    if (pipe(fdUp) == -1) {
        perror("Pipe UP failed!");
        exit(EXIT_FAILURE);
    }

    if (pipe(fdDown) == -1) {
        perror("Pipe DOWN failed!");
        exit(EXIT_FAILURE);
    }

    // Fork a new process
    pid_t pid = fork();
    // Check for errors
    if (pid < 0) {
        perror("Fork failed!");
        exit(EXIT_FAILURE);
    }

    // If we are at the father process
    if (pid > 0) {
        srand(time(NULL));

        close(fdUp[ESCRITA]);
        close(fdDown[LEITURA]);
        int childCredits = STARTING_CREDIT;
        while (childCredits > 0) {
            int randomNumber = (rand() % 5) + 1;  // random number between 1 and 5
            printf("RANDOM NUMBER WAS: %d\n", randomNumber);
            write(fdDown[ESCRITA], &CONTINUE, BUFFER_SIZE);
            int bet;
            read(fdUp[LEITURA], &bet, BUFFER_SIZE);
            if (bet == randomNumber) {
                printf("WON! The child bet was: %d\n", bet);
                childCredits = childCredits + WIN_CREDIT;
            } else {
                printf("LOST! The child bet was: %d\n", bet);
                childCredits = childCredits + LOSE_CREDIT;
            }
            printf("Current Credit is: %d\n", childCredits);
            printf("----------------------------------------\n");
        }
        write(fdDown[ESCRITA], &TERMINATE, BUFFER_SIZE);
        close(fdUp[LEITURA]);
        close(fdDown[ESCRITA]);
    }

    // If we are at the child process
    if (pid == 0) {
        srand(time(NULL) * getpid());

        close(fdUp[LEITURA]);
        close(fdDown[ESCRITA]);
        int shouldContinue = 1;
        while (shouldContinue) {
            read(fdDown[LEITURA], &shouldContinue, BUFFER_SIZE);
            int randomNumber = (rand() % 5) + 1;  // random number between 1 and 5
            write(fdUp[ESCRITA], &randomNumber, BUFFER_SIZE);
        }
        close(fdUp[ESCRITA]);
        close(fdDown[LEITURA]);
        exit(EXIT_SUCCESS);
    }

    // Wait for the process to finish and grap the status
    /*
    int status;
    pid_t pidExit = wait(&status);
    int exitStatus = WEXITSTATUS(status);
    printf("O processo com o PID = %d terminou com valor = %d\n", pidExit, exitStatus);
    */

    exit(EXIT_SUCCESS);
}
