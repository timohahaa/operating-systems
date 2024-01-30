#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    puts("Parent process started working...");
    pid_t self = getpid();
    pid_t parent = getppid();
    pid_t child = fork();
    if (child == 0) {

        // is a child process
        char *env[] = {(char *)"CHILD_ENV=amogus", (char *)NULL};
        execvpe("child", argv, env);

    } else if (child > 0) {

        // is a parent process
        printf("Parent: pid is %d, parent pid is %d, child pid is %d\n", self,
               parent, child);

        int status = 0;
        while (waitpid(child, &status, WNOHANG) == 0) {
            usleep(500000);
        }

        printf("Parent: child process returned exit code: %d\n",
               WEXITSTATUS(status));

    } else {
        perror("fork error");
    }

    puts("Parent process exited.");
    return 0;
}
