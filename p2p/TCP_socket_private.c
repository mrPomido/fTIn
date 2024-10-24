#ifndef FTIN_H
#define FTIN_H
#include "fTin.h"
#endif
#include "TCP_socket_private.h"


TCP_socket* crTCP_socket(SOCKET socket_desc, IPv4 address, char* port, char* peer_name, char* job_name, char* storage){
    TCP_socket* newborn  = (TCP_socket*)malloc(sizeof(TCP_socket));
    newborn->socket_desc = socket_desc;
    newborn->address     = address;
    newborn->port        = (char*)malloc(sizeof(char) * strlen(port));
    newborn->peer_name   = (char*)malloc(sizeof(char) * strlen(peer_name));
    newborn->job_name    = (char*)malloc(sizeof(char) * strlen(job_name));
    newborn->storage     = fopen(storage, "a+");
    if(newborn->port == NULL || newborn->peer_name == NULL || newborn->storage == NULL || newborn->job_name == NULL){
        printf("\033[31mError creating TCP_socket for: %s.\033[0m\n", peer_name);
        newborn->address = TCP_SOCKET_CONSTR_FAILED;
    }
    if(newborn->storage) fprintf(newborn->storage, "\t\t>---CONNECTION CREATED, START LOGGING---<\n");
    strcpy(newborn->port, port);
    strcpy(newborn->peer_name, peer_name);
    strcpy(newborn->job_name, job_name);
    newborn->next = NULL;
    newborn->prev = NULL;
    return newborn;
}

void drTCP_socket(TCP_socket* condemned){
    free(condemned->port);
    free(condemned->peer_name);
    free(condemned->job_name);
    fprintf(condemned->storage, "\t\t>---CONNECTION CLOSED---<\n");
    fclose(condemned->storage);
    CLOSESOCKET(condemned->socket_desc);
    condemned->next = NULL;
    condemned->prev = NULL;    
    free(condemned);
}

void singlify_TCP_socket(TCP_socket* dissected){
    dissected->next = NULL;
    dissected->prev = NULL;
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

char* get_job_name(TCP_socket* dissected){
    return dissected->job_name;
}

char* set_job_name(TCP_socket* dissected, char* job_name){
    dissected->job_name = realloc(dissected->job_name, strlen(job_name));
    strcpy(dissected->job_name, job_name);
    return job_name;
}

FILE* get_storage(TCP_socket* dissected){
    return dissected->storage;
}

FILE* set_storage(TCP_socket* dissected, char* new_storage_name){
    FILE* new_storage = fopen(new_storage_name, "a+");
    if(new_storage == NULL){
        printf("\033[31mError replacing a storage for: %s.\033[0m\n", dissected->peer_name);
        printf("\033[31mFile remains the same.\033[0m\n", dissected->peer_name);
        new_storage = dissected->storage;
    }
    else{
        fclose(dissected->storage);
        dissected->storage = new_storage;
    }
    return new_storage;
}

TCP_socket* get_next_TCPs(TCP_socket* dissected){
    return dissected->next;
}

TCP_socket* get_prev_TCPs(TCP_socket* dissected){
    return dissected->prev;
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
    
    return crTCP_socket(socket_listen, LISTENER, port, "@Listener", "JN_Listen", "listener.ftin");
}

int start_listening(TCP_socket* listener){
    printf("Listening...\n");
    int to_return = 0;
    if(listen(listener->socket_desc, 10) < 0){
        fprintf(stderr, "listen() failed: %d\n", GETSOCKETERRNO());
        to_return = LISTENING_FAILED;
    }
    return to_return;
}