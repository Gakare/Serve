#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void Error(char *Msg)
{
    perror(Msg);
    exit(0);
}

int main(int argc, char **argv)
{
    int SocketFD, NewSocketFD, PortNum, ClientLength;
    char Buffer[256];
    struct sockaddr_in ServerAddress, ClientAddress;
    int n;

    if (argc < 2)
    {
        printf("ERROR: No port provided\n");
        Error("USAGE: `./server.out [port number]`\n");
    }

    SocketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (SocketFD < 0)
    {
        Error("ERROR: Failed to open socket\n");
    }
    bzero((char *)&ServerAddress, sizeof(ServerAddress));
    PortNum = atoi(argv[1]);
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_port = htons(PortNum);
    ServerAddress.sin_addr.s_addr = INADDR_ANY;

    // NOTE: Prompt the user to connect
    printf("Server started...\n");
    printf("Please connect client to server: `client localhost %d`\n", PortNum);

    if (bind(SocketFD, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress)) < 0)
    {
        Error("ERROR: Failed to bind");
    }

    listen(SocketFD, 5);
    ClientLength = sizeof(ClientAddress);
    NewSocketFD = accept(SocketFD, (struct sockaddr *)&ClientAddress, &ClientLength);
    if (NewSocketFD < 0)
    {
        Error("ERROR: Failed to accept");
    }
    else
    {
        printf("Successfully connected...\n");
    }

    // NOTE: Prompting the user to generate 

    bzero(Buffer, 256);
    n = read(NewSocketFD, Buffer, 255);
    if (n < 0)
    {
        Error("ERROR: Failed to read from socket");
    }

    printf("Here is the message: %s\n", Buffer);
    
    n = write(NewSocketFD, "I got your message\n", 18);
    if (n < 0)
    {
        Error("ERROR: Failed to write to socket");
    }

    return (0);
}
