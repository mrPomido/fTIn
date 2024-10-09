#include "../src/headers.h"

int main(){
#if defined(_WIN32)
    WSADATA d;
    if(WSAStartup(MAKEWORD(2, 2), &d)){
        fprintf(srderr, "Failed to initialize.\n");
        return 1;
    }
#endif

    printf("Configuring remote address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_DGRAM;
    struct addrinfo* peer_address;
    if(getaddrinfo("127.0.0.1", "8080", &hints, &peer_address)){
        fprintf(stderr, "getaddrinfo() failed: (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    printf("Remote addres is: ");
    char address_buffer[100];
    char service_buffer[100];
    getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen,
                address_buffer, sizeof(address_buffer),
                service_buffer, sizeof(address_buffer),
                NI_NUMERICHOST | NI_NUMERICSERV);
    printf("%s %s\n", address_buffer, service_buffer);

    printf("Creating socket...\n");
    SOCKET socket_peer;
    socket_peer = socket(peer_address->ai_family,
                         peer_address->ai_socktype,
                         peer_address->ai_protocol);
    if(!ISVALIDSOCKET(socket_peer)){
        fprintf(stderr, "socket() failed: (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    const char* message = "Hello World, brooooooo\nI'm bluee ba du die ba duu daiii, ba du di, la lu laaai";
    printf("Sending: %s\n", message);
    int bytes_sent = sendto(socket_peer, message, strlen(message), 0, peer_address->ai_addr, peer_address->ai_addrlen);
    printf("Sent %d bytes.\n", bytes_sent); 

    char read[1024];
    struct sockaddr_storage server_address;
    socklen_t server_len = sizeof(server_address);
    int bytes_resieved = recvfrom(socket_peer, read, sizeof(read), 0, (struct sockaddr*)&server_address, &server_len);
    printf("%s\n", read);

    freeaddrinfo(peer_address);
    CLOSESOCKET(socket_peer);

#if defined(_WIN32)
    WSACleanup();
#endif

    printf("Finished");

    return 0;
}