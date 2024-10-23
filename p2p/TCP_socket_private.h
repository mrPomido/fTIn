struct TCP_socket{
    SOCKET socket_desc; // SOCKET descriptor
    IPv4 address; // Address (connected node)
    char* port; // Service number (this node)
    char* peer_name; // Connected peer name
    char* job_name; // Name for task, working on socket
    FILE* storage; // FILE descriptor (storage of sent and received data)
    TCP_socket* prev;
    TCP_socket* next;
};

typedef struct TCP_socket TCP_socket;

