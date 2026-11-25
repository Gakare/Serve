#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>

void Error(char *Msg)
{
    perror(Msg);
    exit(0);
}

int main(int argc, char **argv)
{
    int SocketFD, PortNum, n;
    struct sockaddr_in ServerAddress;
    struct hostent *Server;
    char Buffer[256];
    clock_t TimerStart, TimerEnd;

    if (argc < 3)
    {
        fprintf(stderr, "Usage %s hostname port\n", argv[0]);
        exit(0);
    }

    PortNum = atoi(argv[2]);

    SocketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (SocketFD < 0)
    {
        Error("ERROR: failed to open socket\n");
    }

    Server = gethostbyname(argv[1]);
    if (Server == NULL)
    {
        Error("ERROR: No host found\n");
    }

    printf("Host IP1 connecting to IP2...\n");
    bzero((char *)&ServerAddress, sizeof(ServerAddress));
    ServerAddress.sin_family = AF_INET;
    bcopy((char *)Server->h_addr,
            (char *)&ServerAddress.sin_addr.s_addr,
            Server->h_length);

    ServerAddress.sin_port = htons(PortNum);
    if(connect(SocketFD, (struct sockaddr *)&ServerAddress,
                sizeof(ServerAddress)) < 0)
    {
        Error("ERROR: failed to connect\n");
    }

    printf("Successfully connected...\n");

    // NOTE: Record starting time of the first prompt
    TimerStart = clock();
    printf("Please enter 10 numbers: ");
    bzero(Buffer, 256);
    fgets(Buffer, 255, stdin);
    n = write(SocketFD, Buffer, strlen(Buffer));

    if (n < 0)
    {
        Error("ERROR: failed to write to socket\n");
    }

    bzero(Buffer, 256);
    n = read(SocketFD, Buffer, 255);
    if (n < 0)
    {
        Error("ERROR: failed to read from socket");
    }
    printf("%s\n", Buffer);

    return (0);
}
