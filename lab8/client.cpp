#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <pthread.h>
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
#include <vector>
// struct sockaddr_in serverAddr;
// serverAddr.sin_family = AF_INET;
// serverAddr.sin_port = htons(7000);
// serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
// int result = connect(listen_socket, (struct sockaddr *)&serverAddr,
//                      sizeof(serverAddr));

int soc = 0;

pthread_t request_id, receive_id, wait_id;

int request_flag = 0;
int receive_flag = 0;
int wait_flag = 0;

void *request(void *args) {
    int request_id = 0;
    while (request_flag == 0) {
        char req[100] = {0};
        sprintf(req, "%d", request_id++);
        int ret = send(soc, req, sizeof(req), 0);
        if (ret == -1) {
            perror("send");
        } else {
            // ok
        }
        printf("Sent request %s\n", req);
        sleep(1);
    }
    pthread_exit((void *)7);
}

void *receive(void *args) {
    while (receive_flag == 0) {
        char resp[100];
        int ret = recv(soc, (void *)&resp, sizeof(resp), 0);
        if (ret == -1) {
            // error
            perror("recv");
            sleep(1);
        } else if (ret == 0) {
            // disconnected
            sleep(1);
        } else {
            printf("Got responce: %s\n", resp);
        }
    }
    pthread_exit((void *)8);
}

void *wait(void *args) {
    while (wait_flag == 0) {
        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = ntohs(11111);
        serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        int result =
            connect(soc, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
        if (result == -1) {
            perror("connect");
            sleep(1);
        } else {
            // connected
            struct sockaddr_in soc_addr;
            socklen_t len = sizeof(soc_addr);
            getsockname(soc, (struct sockaddr *)&soc_addr, &len);

            char ip[INET_ADDRSTRLEN];
            uint16_t port;
            inet_ntop(AF_INET, &soc_addr, ip, sizeof(ip));
            port = htons(soc_addr.sin_port);
            printf("Connected to server, socket address is %s:%d\n", ip, port);
            // make accept and process threads
            pthread_create(&request_id, NULL, request, NULL);
            pthread_create(&receive_id, NULL, receive, NULL);
            pthread_exit((void *)8);
        }
    }
    pthread_exit((void *)1337);
}

int main() {
    puts("Client program started working...");
    soc = socket(AF_INET, SOCK_STREAM, 0);
    int optval = 1;
    setsockopt(soc, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    // struct sockaddr_in addr;
    // addr.sin_port = htons(8081);
    // bind(soc, (struct sockaddr *)&addr, sizeof(addr));

    pthread_create(&wait_id, NULL, wait, NULL);
    puts("Client program is waiting for keyboard input...");
    getchar();
    puts("Keyboard input received!");

    request_flag = 1;
    receive_flag = 1;
    wait_flag = 1;

    pthread_join(request_id, NULL);
    pthread_join(receive_id, NULL);
    pthread_join(wait_id, NULL);

    shutdown(soc, SHUT_RDWR);
    close(soc);

    puts("Client program finished working!");
    return 0;
}
