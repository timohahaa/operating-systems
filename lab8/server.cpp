#include <errno.h>
#include <fcntl.h>
#include <iostream>
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

int listen_socket = 0;
int resp_socket = 0;
std::vector<std::string> queue;
int accept_flag = 0;
int process_flag = 0;
int wait_flag = 0;

void *accept(void *args) {
    while (accept_flag == 0) {
    }
}

void *process(void *args) {
    while (process_flag == 0) {
    }
}

void *wait(void *args) {
    while (wait_flag == 0) {
    }
}

int main() {
    pthread_t accept_id, process_id, wait_id;

    return 0;
}
