#include "fTin.h"

int main(int argc, char** argv){
    TCP_socket* listener = listener_create("8080");
    crTCP_socket_list(listener);
    int listener_state = start_listening(listener);
    
    


    return 0;
}
