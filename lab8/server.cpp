#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <pthread.h>
#include <queue>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

int listen_socket = 0;
int client_socket = 0;
int resp_socket = 0;
std::queue<std::string> queue;
int accept_flag = 0;
int process_flag = 0;
int wait_flag = 0;

pthread_mutex_t mutex;
pthread_t accept_id, process_id;
pthread_t wait_id;

void *accept(void *args) {
    while (accept_flag == 0) {
        char data[100];
        int ret = recv(client_socket, (void *)&data, sizeof(data), 0);
        if (ret == -1) {
            // error
            perror("recv");
            sleep(1);
        } else if (ret == 0) {
            // disconnected
            puts("Client disconected, exiting...");
            pthread_exit((void *)69);
        } else {
            pthread_mutex_lock(&mutex);
            queue.push(std::string(data));
            pthread_mutex_unlock(&mutex);
            printf("Got request: %s\n", data);
        }
    }
    pthread_exit((void *)69);
}

void *process(void *args) {
    while (process_flag == 0) {
        pthread_mutex_lock(&mutex);
        if (!queue.empty()) {
            std::string req = queue.front();
            queue.pop();
            pthread_mutex_unlock(&mutex);
            int data = getdtablesize();
            const char *resp = (req + " " + std::to_string(data)).c_str();
            int ret = send(client_socket, resp, strlen(resp), 0);
            if (ret == -1) {
                perror("send");
            } else {
                // ok
                printf("Sent a responce: %s\n", resp);
            }
        } else {
            pthread_mutex_unlock(&mutex);
        }
    }
    pthread_exit((void *)28);
}

void *wait(void *args) {
    while (wait_flag == 0) {
        struct sockaddr_in client;
        socklen_t len = sizeof(client);
        int result = accept(listen_socket, (struct sockaddr *)&client, &len);
        if (result == -1) {
            perror("connect");
            sleep(1);
        } else {
            // connected
            client_socket = result;
            char ip[INET_ADDRSTRLEN];
            uint16_t port;
            inet_ntop(AF_INET, &client, ip, sizeof(ip));
            port = htons(client.sin_port);
            printf("Client connected, client address is %s:%d\n", ip, port);
            // make accept and process threads
            pthread_create(&accept_id, NULL, accept, NULL);
            pthread_create(&process_id, NULL, process, NULL);
            pthread_exit((void *)8);
        }
    }
    pthread_exit((void *)7);
}

int main() {
    puts("Server program started working...");
    listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    int optval = 1;
    setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &optval,
               sizeof(optval));

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(12345);

    int _ = bind(listen_socket, (struct sockaddr *)&addr, sizeof(addr));

    listen(listen_socket, 1);

    pthread_create(&wait_id, NULL, wait, NULL);
    puts("Server program is waiting for keyboard input...");
    getchar();
    puts("Keyboard input received!");

    accept_flag = 1;
    process_flag = 1;
    wait_flag = 1;

    pthread_join(accept_id, NULL);
    pthread_join(process_id, NULL);
    pthread_join(wait_id, NULL);

    shutdown(client_socket, SHUT_RDWR);
    close(client_socket);
    close(listen_socket);

    puts("Server program finished working!");
    return 0;
}
