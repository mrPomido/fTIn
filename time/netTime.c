#include "../src/headers.h"

#include <time.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]){
#if defined(_WIN32)
    WSADATA d;
    if(WSAStartup(MAKEWORD(2, 2), &d)){
        fprintf(stderr, "Failed to initialize.\n");
        return 1;
    }
#endif
    printf("Configuring local adresses...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo* bind_address;
    getaddrinfo(0, "5055", &hints, &bind_address);

    printf("Creating socket...\n");
    SOCKET socket_listen;
    socket_listen = socket(bind_address->ai_family,
                           bind_address->ai_socktype,
                           bind_address->ai_protocol);
    
    if(!ISVALIDSOCKET(socket_listen)){
        fprintf(stderr, "socket() failed: %d\n", GETSOCKETERRNO());
        return 1;
    }

    printf("Binding socket to local address...\n");
    if(bind(socket_listen, 
            bind_address->ai_addr,
            bind_address->ai_addrlen)){
        fprintf(stderr, "bind() failed: %d\n", GETSOCKETERRNO());
        return 1;
    }
    freeaddrinfo(bind_address);

    printf("Listening...\n");
    if(listen(socket_listen, 10) < 0){
        fprintf(stderr, "listen() failed: %d\n", GETSOCKETERRNO());
        return 1;
    }

    printf("Waiting for connection...\n");
    struct sockaddr_storage client_address;
    socklen_t client_len = sizeof(client_address);
    SOCKET socket_client = accept(socket_listen, (struct sockaddr*)&client_address, &client_len);
    if(!ISVALIDSOCKET(socket_client)){
        fprintf(stderr, "accept() failed: %d\n", GETSOCKETERRNO());
        return 1;
    }
    printf("Client is connected...\n");
    
    char address_buffer[100];
    getnameinfo((struct sockaddr*)&client_address,
                 client_len,
                 address_buffer,
                 sizeof(address_buffer),
                 0, 0,
                 NI_NUMERICHOST);
    printf("%s\n", address_buffer);

    printf("Reading request...\n");
    char request[1024];
    int bytes_recieved = recv(socket_client, request, 1024, 0);
    printf("Recieved %d bytes.\n", bytes_recieved);
    printf("%.*s\n", bytes_recieved, request);

    printf("Sending response...\n");
    const char* response = 
        "HTTP/1.1 200 OK\r\n"
        "Connection: close\r\n"
        "Content type: text\r\n\r\n"
        "Local time is: ";
    int bytes_sent = send(socket_client, response, strlen(response), 0);
    printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(response));

    time_t timer;
    time(&timer);
    char* time_msg = ctime(&timer);
    bytes_sent = send(socket_client, "Olya, ya tebya lublu!\0", 21, 0);
    printf("Sent %d of %d bytes\n", bytes_sent, (int)strlen(time_msg));

    printf("Closing connection...\n");
    CLOSESOCKET(socket_client);

    printf("Cosing listening socket...\n");
    CLOSESOCKET(socket_listen);

#if defined(_WIN32)
    WCACleanup();
#endif

    printf("Finished\n");
    
    return 0;
}