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

const int MESSAGE_SIZE = sizeof(int) + 1;
const char *fifo_path = "/tmp/amogus_fifo";
int flag = 0;
int fifo_fd;

void clear_buffer(char buffer[MESSAGE_SIZE]) {
    for (int i = 0; i < MESSAGE_SIZE; i++) {
        buffer[i] = 0;
    }
}

void *proc(void *args) {
    char message_buffer[MESSAGE_SIZE];
    while (flag == 0) {
        clear_buffer(message_buffer);
        ssize_t ret = read(fifo_fd, message_buffer, MESSAGE_SIZE);
        if (ret == 0) {
            // writer disconnected OR unavailable
            puts("Writer program is not available");
            puts("Press any key to exit");
        } else if (/*errno == EBADF*/ ret == -1) {
            puts("Writer has not written anything");
        } else {
            puts(message_buffer);
        }
        fflush(stdout);
        sleep(1);
    }
    pthread_exit((void *)8);
}

int main() {
    puts("Reader program started working...");
    pthread_t id;
    mkfifo(fifo_path, S_IRWXU);
    fifo_fd = open(fifo_path, O_RDONLY | O_NONBLOCK);

    pthread_create(&id, NULL, proc, NULL);

    puts("Reader program is waiting for keyboard input...");
    getchar();
    puts("Keyboard input received!");

    flag = 1;

    int ret = 0;
    pthread_join(id, (void **)&ret);

    close(fifo_fd);
    unlink(fifo_path);

    puts("Reader program finished working!");
    return 0;
}
