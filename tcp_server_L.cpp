#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int get_information_from_client(int cs)
{
    //char* int_recv_buffer = (char*)malloc(sizeof(int));
    char int_recv_buffer[10]; 
    int result_recv;
    char fb_msg[] = "put";
    
    // send first msg PUT

    if (result_recv = recv(cs, int_recv_buffer, sizeof(char) * 3, 0) < 0){
        printf("ERROR PUT MESSAGE\n");
        return -1;
    }

    printf("%s\n", int_recv_buffer);
    return 1;

    // send number message


}


int set_non_block_mode(int s)
{
int fl = fcntl(s, F_GETFL, 0);
return fcntl(s, F_SETFL, fl | O_NONBLOCK);
}


int main()
{
    int connectSocket = -1;
    struct sockaddr_in addr = {0};

    connectSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (connectSocket < 0){
        printf("ERROR SOCKET CONNECT\n");
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9000);
    //addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int result_bind = bind(connectSocket, (struct sockaddr*) &addr, sizeof(addr));

    if (result_bind < 0){
        printf("ERROR BIND SOCKET TO ADDRESS\n");
        return -1;
    }

    if (listen(connectSocket, 10) < 0){
        printf("ERROR LISTEN SOCKET\n");
        return -1;
    }


    do {
        socklen_t addrlen = sizeof(addr);
        int cs = accept(connectSocket, (struct sockaddr*) &addr, &addrlen);
        unsigned int ip_client;



        if (cs < 0){
            printf("ERROR ACCEPT CONNECT CLIENT\n");
            return -1;
        }

        ip_client = ntohl(addr.sin_addr.s_addr);
        printf(" Client connected: %u.%u.%u.%u ", 
            (ip_client >> 24) & 0xFF, (ip_client >> 16) & 0xFF, (ip_client >> 8) & 0xFF, (ip_client) & 0xFF);

        // GEt information from tcp_client

        get_information_from_client(cs);

        // func to get string


        // func to send "ok"

        // s_close(cs);

    }

    while (1);

    



    
}