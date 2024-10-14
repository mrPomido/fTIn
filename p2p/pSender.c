#include "../src/headers.h"

int main(){
#if defined(_WIN32)
    WSADATA d;
    if(WSAStartup(MAKEWORD(2, 2), &d)){
        fprintf(stderr, "Failed to initialize.\n");
        return 0;
    }
#endif

    printf("Configuring local address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    struct addrinfo* bind_address;
    getaddrinfo(0, "5050", &hints, &bind_address);

    printf("Creating socket...\n");
    SOCKET socket_listen = socket(bind_address->ai_family,
                                  bind_address->ai_socktype,
                                  bind_address->ai_protocol);
    if(!ISVALIDSOCKET(socket_listen)){
        fprintf(stderr, "socket() failed: (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    printf("Binding socket to local address...\n");
    if(bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen)){
        fprintf(stderr, "bind() failed: (%d)\n", GETSOCKETERRNO());
        return 1;
    }
    freeaddrinfo(bind_address);

    printf("Listening...\n");
    if(listen(socket_listen, 10) < 0){
        fprintf(stderr, "listen() failed: (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    fd_set master;
    fd_set readAv;
    fd_set writeAv;
    FD_ZERO(&master);
    FD_ZERO(&readAv);
    FD_ZERO(&writeAv);
    FD_SET(socket_listen, &master);
    SOCKET max_socket = socket_listen;

    printf("Waiting for connection...\n");
    while(1){
        readAv = master;
        writeAv = master;
        if(select(max_socket + 1, &readAv, &writeAv, 0, 0) < 0){
            fprintf(stderr, "select() failed: (%d)\n", GETSOCKETERRNO());
            return 1;
        }
        SOCKET i;
        for(i = 1; i <= max_socket; i++){
            if(FD_ISSET(i, &readAv)){
                if(i == socket_listen){
                    struct sockaddr_storage other_address;
                    socklen_t other_len = sizeof(other_address);
                    SOCKET socket_other = accept(socket_listen, (struct sockaddr*)&other_address, &other_len);
                    if(!ISVALIDSOCKET(socket_other)){
                        fprintf(stderr, "accept() failed: (%d)\n", GETSOCKETERRNO());
                        return 1;
                    }
                    FD_SET(socket_other, &master);
                    if(socket_other > max_socket) max_socket = socket_other;
                    char address_buffer[100];
                    /* char serv_buffer[100]; */
                    getnameinfo((struct sockaddr*)&other_address, other_len, 
                                address_buffer, sizeof(address_buffer), 
                                0, 0, /*serv_buffer,    sizeof(serv_buffer),*/ NI_NUMERICHOST);
                    printf("New connection from: %s \n", address_buffer/* , serv_buffer */);
                }
                else{
                    char buffer[1024];
                    int bytes_received = recv(i, buffer, sizeof(buffer), 0);
                    if(bytes_received < 1){
                        FD_CLR(i, &master);
                        CLOSESOCKET(i);
                        continue;
                    }
                    printf("Receive: \"%.*s\"\n", bytes_received, buffer);
                }
            }
        }
    }










#if defined(_WIN32)
    WSACleanup();
#endif


    return 0;
}