#ifndef FTIN_H
#define FTIN_H
#include "fTin.h"
#endif
#include "TCP_socket_private.h"

typedef struct{
    TCP_socket* head;
    TCP_socket* tail;
}TCP_sock_list;

TCP_socket_list* crTCP_socket_list(TCP_socket* TCPs){
    TCP_sock_list* list = (TCP_sock_list*)malloc(sizeof(TCP_sock_list));
    list->head = TCPs;
    list->tail = TCPs;
    return list;
}

void add_TCP_socket_to_list(TCP_sock_list* sock_list, TCP_socket* TCPs){
    TCPs->prev = sock_list->tail;
    TCPs->next = NULL;
    sock_list->tail = TCPs;
}

TCP_socket* find_TCP_socket_in_list(TCP_sock_list* list, TCP_socket* TCPs){
    for(TCP_socket* slider = list->head; slider != NULL; slider = slider->next){
        if(slider == TCPs) break;
    }
    return slider;
}

TCP_socket* cut_TCP_socket(TCP_sock_list* list, TCP_socket* TCPs){
    TCP_socket* cut_out = find_TCP_socket_in_list(list, TCPs);
    if(cut_out != NULL){
        if(cut_out == list->head){
            list->head = cut_out->next;
            (list->head)->prev = NULL;
        }
        else if(cut_out == list->tail){
            list->tail = cut_out->prev;
            (list->tail)->next = NULL;
        }
        else{
            (TCPs->prev)->next = TCPs->next;
            (TCPs->next)->prev = TCPs->prev;
        }
        singlify_TCP_socket(TCPs);
    }
    return cut_out;
}

void drTCP_socket_list(TCP_sock_list* list){
    while(list->tail != NULL){
        drTCP_socket(cut_TCP_socket(list, list->tail));
    }
}