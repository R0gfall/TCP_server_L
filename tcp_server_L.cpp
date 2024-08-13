#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>

#include <stdio.h>
#include <string.h>


int get_information_from_client(int cs)
{
    
}





int main()
{
    int connectSocket = -1;
    struct sockaddr_in addr;
    printf("1111\n");

    connectSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (connectSocket < 0){
        printf("ERROR SOCKET CONNECT\n");
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9000);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);


    if (bind(connectSocket, (struct sockaddr*) &addr, sizeof(addr)) < 0){
        printf("ERROR BIND SOCKET TO ADDRESS\n");
        return -1;
    }

    if (listen(connectSocket, 1) < 0){
        printf("ERROR LISTEN SOCKET\n");
        return -1;
    }

    printf("22222\n");

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

        // func to get string


        // func to send "ok"

        // s_close(cs);

    }

    while (1);

    



    
}