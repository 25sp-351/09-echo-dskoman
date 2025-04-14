#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
    // begin reading arguments

    // arg[0] == program name
    // arg[1] == port number
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // read the port number
    int port = strtol(argv[1], NULL, 10);
    if (port <= 0 || port > 65535)
    {
        fprintf(stderr, "invalid port number: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // end reading arguments

    // create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in socket_address;
    memset(&socket_address, 0, sizeof(socket_address));
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
    socket_address.sin_port = htons(port);

    printf("binding to port %d\n", port);

    int return_val;

    return_val = bind(sockfd, (struct sockaddr *)&socket_address, sizeof(socket_address));
    if (return_val < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }
}
