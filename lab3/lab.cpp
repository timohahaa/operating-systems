#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int channel[2];

typedef struct {
    int flag;
    char symbol;
} thread_args;

void create_channel(int option) {
    switch (option) {
    case 1: {
        pipe(channel);
    } break;
    case 2: {
        pipe2(channel, O_NONBLOCK);
    } break;
    case 3: {
        pipe(channel);
        fcntl(channel[0], F_SETFL, O_NONBLOCK);
        fcntl(channel[1], F_SETFL, O_NONBLOCK);
    } break;
    }
}

void *proc1(void *args) {
    puts("Thread 1 started working...");
    thread_args *targs = (thread_args *)args;

    while (targs->flag == 0) {
        int data = getdtablesize();
        ssize_t ret = write(channel[1], (void *)&data, sizeof(int));
        if (ret == 0) {
            puts("End of file");
        } else if (ret == -1) {
            perror("err");
        } else if (ret > 0) {
            puts("Message succesfully written!");
        }
        sleep(1);
    }

    puts("Thread 1 finished working!");
    return 0;
}

void *proc2(void *args) {
    puts("Thread 2 started working...");
    thread_args *targs = (thread_args *)args;

    int msg = 0;
    while (targs->flag == 0) {
        ssize_t ret = read(channel[0], (void *)&msg, sizeof(int));
        if (ret == 0) {
            puts("End of file");
        } else if (ret == -1) {
            perror("err");
        } else if (ret > 0) {
            puts("Message succesfully received:");
            printf("%d\n", msg);
        }
        sleep(1);
    }
    puts("Thread 2 finished working!");
    return 0;
}

int main(int argc, char *argv[]) {
    puts("Main program started working...");
    if (argc != 2) {
        puts("wrong number of arguments!");
        puts("specify a way to create a channel: 1, 2 or 3");
        return 1;
    } else {
        int opt = atoi(argv[1]);
        create_channel(opt);
    }
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

    close(channel[0]);
    close(channel[1]);
    puts("Main program finished working!");
    return 0;
}
