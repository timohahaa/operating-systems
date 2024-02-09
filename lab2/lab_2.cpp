#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

sem_t sem;

typedef struct {
    int flag;
    char symbol;
} thread_args;

void *proc1(void *args) {
    puts("Thread 1 started working...");
    thread_args *targs = (thread_args *)args;
    while (targs->flag == 0) {

        sem_wait(&sem);
        for (int i = 0; i < 10; i++) {
            putchar(targs->symbol);
            fflush(stdout);
            sleep(1);
        }
        sem_post(&sem);

        sleep(1);
    }

    puts("Thread 1 finished working!");
    return 0;
}

void *proc2(void *args) {
    puts("Thread 2 started working...");
    thread_args *targs = (thread_args *)args;
    while (targs->flag == 0) {

        sem_wait(&sem);
        for (int i = 0; i < 10; i++) {
            putchar(targs->symbol);
            fflush(stdout);
            sleep(1);
        }
        sem_post(&sem);

        sleep(1);
    }

    puts("Thread 2 finished working!");
    return 0;
}

int main() {
    puts("Main program started working...");

    sem_init(&sem, 0, 1);
    pthread_t id1;
    pthread_t id2;

    thread_args args1;
    thread_args args2;
    args1.flag = 0;
    args1.symbol = '1';
    args2.flag = 0;
    args2.symbol = '2';

    pthread_create(&id1, NULL, proc1, &args1);
    pthread_create(&id2, NULL, proc2, &args2);

    puts("Main program is waiting for keyboard input...");
    getchar();
    puts("Keyboard input received!");

    args1.flag = 1;
    args2.flag = 1;

    pthread_join(id1, NULL);
    pthread_join(id2, NULL);

    sem_destroy(&sem);

    puts("Main program finished working!");
    return 0;
}
