#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    char *args[] = {"/bin/ls", "-l", NULL};
    
    int status = execv("/bin/ls", args);
    
    if (status == -1) {
        perror("execv failed");
        exit(1);
    }
    
    return 0;
}
