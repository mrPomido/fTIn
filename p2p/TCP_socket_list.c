#ifndef FTIN_H
#define FTIN_H
#include "fTin.h"
#endif

typedef struct{
    TCP_socket* head;
    TCP_socket* tail;
}TCP_sock_list;

void add_TCP_socket_to_list(TCP_sock_list* sock_list, TCP_socket* TCPs){
    if(sock_list->head == NULL){
        sock_list->head = TCPs;
        sock_list->tail = TCPs;
    }
    else{
        TCPs->prev = tail;
        TCP->next = NULL;
        sock_list->tail = TCPs;
    } 
}
TCP_socket* cut_TCP_socket(TCP_sock_list* TCPs){
    if(TCPs->prev) (TCPs->prev)->next = TCPs->next;
    if(TCPs->next) (TCPs->next)->prev = TCPs->prev;
    (TCPs->prev) = NULL;
    (TCPs->next) = NULL;
    return TCPs;
}

TCP_socket* del_TCP_socket(TCP_sock_list* TCPs){

}
