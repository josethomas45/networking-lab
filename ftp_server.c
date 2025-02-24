#include<stdio.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
    FILE *fp;
    int sd, newsd, bd, port, clilen, n;
    char rcv[100], fileread[100];
    struct sockaddr_in servaddr, cliaddr;

    printf("Enter the port address: ");
    scanf("%d", &port);

    // Create socket
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if(sd < 0) {
        printf("Socket creation failed\n");
        exit(1);
    }
    printf("Socket created\n");

    // Configure server address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    // Bind the socket
    bd = bind(sd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if(bd < 0) {
        printf("Binding failed\n");
        exit(1);
    }
    printf("Binding successful\n");

    // Listen for client connections
    listen(sd, 5);
    clilen = sizeof(cliaddr);

    // Accept client connection
    newsd = accept(sd, (struct sockaddr *)&cliaddr, &clilen);
    if(newsd < 0) {
        printf("Acceptance failed\n");
        exit(1);
    }
    printf("Client connected\n");

    // Receive file name from client
    n = recv(newsd, rcv, 100, 0);
    rcv[n] = '\0';

    fp = fopen(rcv, "r");
    if(fp == NULL) {
        send(newsd, "error", 5, 0);
        close(newsd);
        printf("Error: File not found\n");
    } else {
        while(fgets(fileread, sizeof(fileread), fp)) {
            if(send(newsd, fileread, sizeof(fileread), 0) < 0) {
                printf("Error: Unable to send file contents\n");
                break;
            }
            sleep(1);
        }
        fclose(fp);

        send(newsd, "completed", 10, 0);
        printf("File transfer completed\n");
    }

    close(newsd);
    close(sd);
    return 0;
}

