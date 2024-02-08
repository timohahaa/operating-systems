#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

typedef struct {
    int flag;
    char symbol;
} thread_args;

void *proc1(void *args) {
    puts("Thread 1 started working...");
    thread_args *targs = (thread_args *)args;
    while (targs->flag == 0) {
        putchar(targs->symbol);
        fflush(stdout);
        sleep(1);
    }

    puts("Thread 1 finished working!");
    pthread_exit((void *)7);
}

void *proc2(void *args) {
    puts("Thread 2 started working...");
    thread_args *targs = (thread_args *)args;
    while (targs->flag == 0) {
        putchar(targs->symbol);
        fflush(stdout);
        sleep(1);
    }

    puts("Thread 2 finished working!");
    pthread_exit((void *)8);
}

int main() {
    puts("Main program started working...");
    pthread_t id1;
    pthread_t id2;

    thread_args args1;
    thread_args args2;
    args1.flag = 0;
    args1.symbol = '1';
    args2.flag = 0;
    args2.symbol = '2';

    pthread_attr_t attr;
    pthread_attr_init(&attr);

    int err = pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN - 1000);

    if (err != 0) {
        perror("error creating thread: ");
        return 1;
    }
    int ret = pthread_create(&id1, &attr, proc1, &args1);
    if (ret != 0) {
        perror("error creating thread: ");
        return 1;
    }
    pthread_create(&id2, NULL, proc2, &args2);

    puts("Main program is waiting for keyboard input...");
    getchar();
    puts("Keyboard input received!");

    args1.flag = 1;
    args2.flag = 1;

    int ret1 = 0, ret2 = 0;

    pthread_join(id1, (void **)&ret1);
    pthread_join(id2, (void **)&ret2);

    printf("Thread 1 returned: %d, thread 2 returned: %d\n", ret1, ret2);

    puts("Main program finished working!");
    return 0;
}
