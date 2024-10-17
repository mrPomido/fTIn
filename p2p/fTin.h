#ifndef HEADERS_H
#define HEADERS_H
#include "../src/headers.h"
#endif

#define LISTENER -1
#define TCP_SOCKET_CONSTR_FAILED -2


typedef int IPv4;
typedef struct TCP_socket TCP_socket;
struct TCP_socket_list;

SOCKET get_socket(TCP_socket* dissected);
IPv4   get_address(TCP_socket* dissected);
FILE*  get_storage(TCP_socket* dissected);
char*  get_port(TCP_socket* dissected);
char*  get_peer_name(TCP_socket* dissected);

TCP_socket* listener_create(char* port);