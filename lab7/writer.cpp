#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

const int MESSAGE_SIZE = 100;
const char *fifo_path = "/tmp/amogus_fifo";
pthread_t open_id;
pthread_t write_id;
int open_thread_flag = 0;
int write_thread_flag = 0;
int fifo_fd;

void get_message(char msgBuffer[MESSAGE_SIZE], int data) {
    sprintf(msgBuffer, "Descriptor table size is: %d", data);
}

void clear_buffer(char buffer[MESSAGE_SIZE]) {
    for (int i = 0; i < MESSAGE_SIZE; i++) {
        buffer[i] = 0;
    }
}

void *write_thread(void *args) {
    char message_buffer[MESSAGE_SIZE];
    while (write_thread_flag == 0) {
        clear_buffer(message_buffer);
        int data = getdtablesize();
        get_message(message_buffer, data);
        ssize_t ret = write(fifo_fd, message_buffer, MESSAGE_SIZE);
        puts(message_buffer);
        if (ret == 0) {
            puts("Reader program is not available");
        } else if (/*errno == EBADF || errno*/ ret == -1) {
            // reader disconnected
            puts("Reader has disconnected");
            pthread_exit((void *)7);
        } else {
            puts("here");
            printf("%d\n", ret);
        }
        sleep(1);
    }
    pthread_exit((void *)8);
}

void *open_thread(void *args) {
    while (open_thread_flag == 0) {
        fifo_fd = open(fifo_path, O_WRONLY | O_NONBLOCK);
        if (fifo_fd == -1) {
            // error
            perror("error oppening fifo");
            sleep(1);
        } else {
            pthread_create(&write_id, NULL, write_thread, NULL);
            pthread_exit((void *)7);
        }
    }
    pthread_exit((void *)8);
}

int main() {
    puts("Writer program started working...");
    mkfifo(fifo_path, S_IRWXU);

    pthread_create(&open_id, NULL, open_thread, NULL);

    puts("Writer program is waiting for keyboard input...");
    getchar();
    puts("Keyboard input received!");

    open_thread_flag = 1;
    write_thread_flag = 1;

    int ret = 0;
    pthread_join(open_id, (void **)&ret);
    pthread_join(write_id, (void **)&ret);

    close(fifo_fd);
    unlink(fifo_path);

    puts("Writer program finished working!");
    return 0;
}
