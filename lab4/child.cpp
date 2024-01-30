#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    puts("Child process started executing...");
    pid_t self = getpid();
    pid_t parent = getppid();
    printf("Child: pid is %d, parent pid is %d\n", self, parent);

    puts("Child: printing cli arguments:");
    for (int i = 0; i < argc; i++) {
        puts(argv[i]);
    }
    puts("Child: printing env variable:");
    char *env = getenv("CHILD_ENV");
    puts(env);

    puts("Child process exited.");
    exit(8);
}
