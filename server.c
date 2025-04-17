#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#define DEFAULT_PORT 80
#define MAX_CONNECTIONS 5
#define BUFFER_SIZE 128

void handleConnection(int *sock_fd_ptr)
{
    printf("handling connection on %ls\n", sock_fd_ptr);
    while (1)
    {
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, sizeof(buffer));

        // read from the socket
        int bytes_read = read(*sock_fd_ptr, buffer, sizeof(buffer) - 1);
        if (bytes_read <= 0)
        {
            printf("client disconnected\n");
            break;
        }

        // print the request
        printf("request: %s", buffer);

        // send a response
        char *response = buffer;

        write(*sock_fd_ptr, response, strlen(response));
    }
    printf("done handling connection from %ls\n", sock_fd_ptr);
}

int main(int argc, char *argv[])
{
    int port = DEFAULT_PORT;

    // begin reading arguments

    // arg[0] == program name
    // arg[1] == port number (optional)
    //
    // if no port number is given, use 80
    if (argc < 2)
    {
        printf("starting server on default port (80)\n");
        fprintf(stderr, "to launch with custom port: %s <port>\n", argv[0]);
    }
    else if (argc == 2)
    {
        // parse the port number if supplied
        port = strtol(argv[1], NULL, 10);
        if (port <= 0 || port > 65535)
        {
            fprintf(stderr, "invalid port number: %s\n", argv[1]);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        fprintf(stderr, "invalid number of arguments\n");
        printf("to launch with custom port: %s <port>\n", argv[0]);
        printf("to start with default port (80): %s\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // end reading arguments

    // create a socket
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in socket_address;
    memset(&socket_address, 0, sizeof(socket_address));
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
    socket_address.sin_port = htons(port);

    printf("binding to port %d\n", port);

    int return_val;

    // bind the socket to the address
    return_val = bind(sock_fd, (struct sockaddr *)&socket_address, sizeof(socket_address));
    if (return_val < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // you should be watching...
    return_val = listen(sock_fd, MAX_CONNECTIONS);
    if (return_val < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("listening for connections...\n");

    // accept a connection
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);

    // start the event loop
    while (1)
    {
        pthread_t thread;
        int *client_fd_buffer = malloc(sizeof(int));

        *client_fd_buffer = accept(sock_fd, (struct sockaddr *)&client_address, &client_address_len);

        printf("accepted connection from %s:%d\n",
               inet_ntoa(client_address.sin_addr),
               ntohs(client_address.sin_port));

        pthread_create(&thread, NULL, (void *)handleConnection, (void *)client_fd_buffer);
    }

    return 0;
}
