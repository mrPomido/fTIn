#ifndef FTIN_H
#define FTIN_H
#include "fTin.h"
#endif

struct TCP_socket{
    SOCKET socket_desc; // SOCKET descriptor
    IPv4 address; // Address (connected node)
    char* port; // Service number (this node)
    char* peer_name; // Connected peer name
    FILE* storage; // FILE descriptor (storage of sent and received data)
};

typedef struct TCP_socket TCP_socket;

TCP_socket* TCP_socket_constructor(SOCKET socket_desc, IPv4 address, char* port, char* peer_name, char* storage);
void TCP_socket_destructor(TCP_socket* condemned);
TCP_socket* listener_create(char* service);
SOCKET get_socket(TCP_socket* dissected);
IPv4 get_address(TCP_socket* dissected);
char* get_port(TCP_socket* dissected);
char* get_peer_name(TCP_socket* dissected);
FILE* get_storage(TCP_socket* dissected);


TCP_socket* TCP_socket_constructor(SOCKET socket_desc, IPv4 address, char* port, char* peer_name, char* storage){
    TCP_socket* newborn = (TCP_socket*)malloc(sizeof(TCP_socket));
    newborn->socket_desc = socket_desc;
    newborn->address     = address;
    newborn->port        = (char*)malloc(sizeof(char) * strlen(port));
    newborn->peer_name   = (char*)malloc(sizeof(char) * strlen(peer_name));
    newborn->storage     = fopen(storage, "a+");
    if(newborn->port == NULL || newborn->peer_name == NULL || newborn->storage == NULL){
        printf("\033[31mError creating TCP_socket for: %s.\033[0m\n", peer_name);
        newborn->address = TCP_SOCKET_CONSTR_FAILED;
    }
    if(newborn->storage) fprintf(newborn->storage, "\t\t>---CONNECTION CREATED, START LOGGING---<\n");
    strcpy(newborn->port, port);
    strcpy(newborn->peer_name, peer_name);
    return newborn;
}

void TCP_socket_destructor(TCP_socket* condemned){
    free(condemned->port);
    free(condemned->peer_name);
    fprintf(condemned->storage, "\t\t>---CONNECTION CLOSED---<\n");
    fclose(condemned->storage);
    CLOSESOCKET(condemned->socket_desc);
    free(condemned);
}

SOCKET get_socket(TCP_socket* dissected){
    return dissected->socket_desc;
}

IPv4 get_address(TCP_socket* dissected){
    return dissected->address;
}

char* get_port(TCP_socket* dissected){
    return dissected->port;
}

char* get_peer_name(TCP_socket* dissected){
    return dissected->peer_name;
}

FILE* get_storage(TCP_socket* dissected){
    return dissected->storage;
}

TCP_socket* listener_create(char* port){
    printf("Configuring local address...\n");
    struct addrinfo  hints;
    struct addrinfo* bind_address;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    getaddrinfo(0, port, &hints, &bind_address);

    printf("Creating socket...\n");
    SOCKET socket_listen;
    socket_listen = socket(bind_address->ai_family,
                           bind_address->ai_socktype,
                           bind_address->ai_protocol);
    if(!ISVALIDSOCKET(socket_listen)){
        fprintf(stderr, "socket failed: (%d)\n", GETSOCKETERRNO());
    }

    printf("Binding socket to local address...\n");
    if(bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen)){
        fprintf(stderr, "bind() failed: (%d)\n", GETSOCKETERRNO());
    }
    freeaddrinfo(bind_address);
    
    return TCP_socket_constructor(socket_listen, LISTENER, port, "@Listener", "listener.ftin");
}