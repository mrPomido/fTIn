#include "fTin.h"

int main(int argc, char** argv){
    TCP_socket* listener = listener_create("8080");
    int listener_state = start_listening(listener);

    


    return 0;
}
