#include "../src/headers.h"

int main(int argc, char** argv){
#if defined(_WIN32)
    WSADATA d;
    if(WSAStartup(MAKEWORD(2,2), &d)){
        fprintf(stderr, "Failed to initialize.\n");
        return 1;
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

    printf("creating socket...\n");
    SOCKET socket_listen = 
    


    return 0;
}