#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main() {
    pid_t pid;

    printf("Before fork: Parent process execution\n");

    pid = fork();

    if (pid < 0) {
    
        fprintf(stderr, "Fork failed\n");
        exit(1);
    } else if (pid == 0) {
        
        printf("Child Process:\n");
        printf("- Child PID: %d\n", getpid());
        printf("- Parent PID: %d\n", getppid());
        exit(0);
    } else {
     
        printf("Parent Process:\n");
        printf("- Parent PID: %d\n", getpid());
        printf("- Child PID: %d\n", pid);
        
        wait(NULL);
    }


    return 0;
}
 