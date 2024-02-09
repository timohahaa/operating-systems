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

int flag = 0;
sem_t *write_sem;
const char *write_sem_name = "/write_sem";
sem_t *read_sem;
const char *read_sem_name = "/read_sem";
int shmem_fd;
void *local_addr = malloc(1000);

void *proc2(void *args) {
    puts("Thread 2 started working...");
    while (flag == 0) {
        sem_wait(write_sem);
        int data = 0;
        memcpy(&data, local_addr, sizeof(int));
        printf("Result is: %d\n", data);
        fflush(stdout);
        sem_post(read_sem);
        sleep(1);
    }

    puts("Thread 2 finished working!");
    pthread_exit((void *)7);
}

int main() {
    puts("Main program 2 started working...");
    pthread_t id;

    shmem_fd = shm_open("/amogus_memory", O_RDWR | O_CREAT, S_IRWXU);
    // потому что записываем getdtablesize()
    ftruncate(shmem_fd, 100);
    local_addr =
        mmap(local_addr, 100, PROT_WRITE | PROT_READ, MAP_SHARED, shmem_fd, 0);
    write_sem = sem_open(write_sem_name, O_CREAT, (mode_t)0777, 1);
    read_sem = sem_open(read_sem_name, O_CREAT, (mode_t)0777, 1);

    pthread_create(&id, NULL, proc2, NULL);

    puts("Main program 2 is waiting for keyboard input...");
    getchar();
    puts("Keyboard input received!");

    flag = 1;

    int ret = 0;
    pthread_join(id, (void **)&ret);

    sem_close(write_sem);
    sem_unlink(write_sem_name);
    sem_close(read_sem);
    sem_unlink(read_sem_name);

    munmap(local_addr, 100);
    close(shmem_fd);
    shm_unlink("/amogus_memory");

    printf("Thread 2 returned: %d\n", ret);

    puts("Main program 2 finished working!");
    return 0;
}
