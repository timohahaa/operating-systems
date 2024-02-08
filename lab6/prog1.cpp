#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

int flag = 0;
sem_t *write_sem;
sem_t *read_sem;
int shmem_fd;
void *local_addr;

void *proc1(void *args) {
    puts("Thread 1 started working...");
    while (flag == 0) {
        int data = getdtablesize();
        printf("Result is: %d\n", data);
        memcpy(local_addr, (void *)&data, sizeof(data));
        fflush(stdout);

        sem_post(write_sem);
        sem_wait(read_sem);
        sleep(1);
    }

    puts("Thread 1 finished working!");
    pthread_exit((void *)7);
    return 0;
}

int main() {
    puts("Main program 1 started working...");
    pthread_t id;

    pthread_create(&id, NULL, proc1, NULL);

    puts("Main program 1 is waiting for keyboard input...");
    getchar();
    puts("Keyboard input received!");

    flag = 1;

    int ret = 0;

    pthread_join(id, (void **)&ret);

    printf("Thread 1 returned: %d\n", ret);

    puts("Main program 1 finished working!");
    return 0;
}
