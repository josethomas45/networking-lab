#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>

int main()
{
    FILE *fp;
    int csd, n, cport, s;
    char name[100], fname[100], rcvg[100];
    struct sockaddr_in servaddr;

    printf("Enter the server port: ");
    scanf("%d", &cport);

    // Create socket
    csd = socket(AF_INET, SOCK_STREAM, 0);
    if(csd < 0) {
        printf("Error: Unable to create socket\n");
        exit(1);
    }
    printf("Socket created\n");

    // Configure server address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(cport);

    // Connect to the server
    if(connect(csd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        printf("Connection failed\n");
        exit(1);
    }
    printf("Connected to server\n");

    // Get file names from user
    printf("Enter the existing file name: ");
    scanf("%s", name);
    printf("Enter the new file name: ");
    scanf("%s", fname);

    // Open the new file for writing
    fp = fopen(fname, "w");
    if(fp == NULL) {
        printf("Error: Unable to open file for writing\n");
        close(csd);
        exit(1);
    }

    // Send the existing file name to server
    send(csd, name, sizeof(name), 0);

    // Receive file contents from server and write to file
    while(1) {
        s = recv(csd, rcvg, 100, 0);
        rcvg[s] = '\0';

        if(strcmp(rcvg, "error") == 0) {
            printf("Error: File not found on server\n");
            break;
        }

        if(strcmp(rcvg, "completed") == 0) {
            printf("File transfer complete\n");
            break;
        }

        fputs(rcvg, fp);
        fputs(rcvg, stdout);
    }

    // Clean up and close
    fclose(fp);
    close(csd);
    return 0;
}

