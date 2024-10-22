#ifndef HEADERS_H
#define HEADERS_H
#include "../src/headers.h"
#endif

#define LISTENER -1
#define TCP_SOCKET_CONSTR_FAILED -2
#define TCP_SOCKET_POINTER_IS_NULL -3


typedef int IPv4;
typedef struct TCP_socket TCP_socket;
typedef struct TCP_socket_list TCP_socket_list;

SOCKET get_socket(TCP_socket* dissected);
IPv4   get_address(TCP_socket* dissected);
char*  get_port(TCP_socket* dissected);
char*  get_peer_name(TCP_socket* dissected);
char*  get_job_name(TCP_socket* dissected);
char*  set_job_name(TCP_socket* dissected, char* job_name);
TCP_socket* get_next_TCPs(TCP_socket* dissected);
TCP_socket* get_prev_TCPs(TCP_socket* dissected);
FILE*  get_storage(TCP_socket* dissected);
FILE*  set_storage(TCP_socket* dissected, char* new_storage_name);

TCP_socket* listener_create(char* port);