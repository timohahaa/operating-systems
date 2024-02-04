#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

const int ENTER = 10;
const char SYMBOL = '1';

int main() {
    puts("Program 1 started working...");
    // make getchar nonblocking
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

    const char *sem_name = "/amogus";
    FILE *file;
    sem_t *sem = sem_open(sem_name, O_CREAT, (mode_t)0777, 0);
    file = fopen("./impostor.txt", "a+");

    fd_set fds;
    FD_SET(STDIN_FILENO, &fds);
    timeval *timeout;
    timeout->tv_sec = 0;
    timeout->tv_usec = 0;

    while (select(1, &fds, NULL, NULL, timeout) == 0) {
        sem_wait(sem);
        for (int i = 0; i < 10; i++) {
            fprintf(file, "%c", SYMBOL);
            putchar(SYMBOL);
            fflush(stdout);
            sleep(1);
        }
        sem_post(sem);

        sleep(1);

        if (select(1, &fds, NULL, NULL, timeout) == 0) {
            break;
        } else {
            continue;
        }
    }

    fclose(file);
    sem_close(sem);
    sem_unlink(sem_name);

    puts("Program 1 finished working!");
    return 0;
}
