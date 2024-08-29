#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N 10
#define PORT 9000



int set_non_block_mode(int s)
{
    int fl = fcntl(s, F_GETFL, 0);
    return fcntl(s, F_SETFL, fl | O_NONBLOCK);
}

//Need to push commit to github


// add func to send OK
int send_ok(int cs)
{
    const char ok_msg[] = "ok";
    int send_bytes;

    send_bytes = send(cs, ok_msg, sizeof(ok_msg), 0);
    if (send_bytes < 0){
        printf("ERROR SEND OK MSG\n");
        return -1;
    }
    
    printf("OK MSG SEND\n");
    return 1;
}


int convert_str_to_int(char* string_number)
{
   /*  int integer_number = atoi(string_number);
    printf("== %s\n", string_number); */

    //int integer_number = ntohl((uint32_t)string_number); 
    return 1;

}


/*
int get_information_from_client(int cs) {
    char recv_buffer_PG[4] = {0};  // Размер 4, чтобы вместить 3 символа и нуль-терминатор
    int int_recv_buffer;
    int result_recv;
    const char fb_msg[] = "put";
    
    // Получаем команду ("put" или "get")
    if ((result_recv = recv(cs, recv_buffer_PG, 3, 0)) < 0) {
        printf("ERROR PUT MESSAGE\n");
        return -1;
    }
    
    recv_buffer_PG[result_recv] = '\0';  // Завершаем строку

    if (strcmp(recv_buffer_PG, fb_msg) == 0) {
        printf("PUT GOT IT\n");
    } else if (strcmp(recv_buffer_PG, "get") == 0) {
        printf("GET GOT IT\n");
    } else {
        printf("Unknown command: %s\n", recv_buffer_PG);
        return -1;
    }

    // Получаем число
    if ((result_recv = recv(cs, &int_recv_buffer, sizeof(int_recv_buffer), 0)) < 0) {
        printf("ERROR NUMBER MESSAGE\n");
        return -1;
    }

    int lenData = ntohl(int_recv_buffer);
    printf("Received number: %d\n", lenData);

    // Здесь вы можете обработать полученное число и использовать его

    // Получаем сообщение указанной длины
    char *buffer_message = (char*)malloc(lenData + 1);
    if (!buffer_message) {
        printf("Memory allocation error\n");
        return -1;
    }

    if ((result_recv = recv(cs, buffer_message, lenData, 0)) < 0) {
        printf("ERROR MESSAGE RECEIVE\n");
        free(buffer_message);
        return -1;
    }
    
    buffer_message[lenData] = '\0';  // Завершаем строку
    printf("MESSAGE GOT IT: %s\n", buffer_message);

    free(buffer_message);
    return 1;
}
*/

int get_information_from_client(int cs)
{
    //char* int_recv_buffer = (char*)malloc(sizeof(int));
    char* recv_buffer_PG = (char*)malloc(sizeof(char) * 3); // or ..sizeof(char * 3)
    
    // maybe need to change char to integer

    //char int_recv_buffer[4] = {0};
    char* int_recv_buffer = (char*)malloc(sizeof(int)); // or int* int_recv_buffer = (int*)malloc(sizeof(int));


    int result_recv, number;
    char fb_msg[] = "put";
    
    // send first msg PUT

    if ((result_recv = recv(cs, recv_buffer_PG, (sizeof(char) * 3), 0)) < 0){
        printf("ERROR PUT MESSAGE\n");
        return -1;
    }
    
    recv_buffer_PG[result_recv] = '\0';


    if (strcmp(recv_buffer_PG, fb_msg) == 0){
        printf("PUT GOT IT: %s\n", recv_buffer_PG);

    }

    else if (strcmp(recv_buffer_PG, "get")){
        printf("GET GOT IT: %s\n", recv_buffer_PG);

    }

    // get number message
    uint32_t avd;



    result_recv = recv(cs, &avd, sizeof(int), 0);

    


    if (result_recv < 0){
        printf("ERROR NUMBER MESSAGE\n");
        return -1;
    }
    else{

        //number = atoi(int_recv_buffer);
        //printf("%s\n", avd);

        printf("%s\n", avd);
        //number = convert_str_to_int(avd);

        printf("NUMBER OF MESSAGE GOT IT: %d, <<%d\n", number, result_recv);

        //func to send ok
        /* if (send_ok(cs) == -1){
            printf("ERROR SENDOK FUNC\n");
            return -1;
        }
 */
    }


    // get date information

    result_recv = recv(cs, &avd, sizeof(int), 0);

    printf("%d\n", avd);
    if (result_recv < 0){
        printf("ERROR GET DATA INFO\n");
        return -1;
    }

    else{


        printf(">>>%d, !!!%d\n", result_recv, ntohl(avd));

        //number = convert_str_to_int(avd);
        printf("DATE GOT IT: %d\n", number);

        // func to write data (need translate code htons or htonl)



        // func to send ok
     /* if (send_ok(cs) == -1){
            printf("ERROR SENDOK FUNC\n");
            return -1;
        } */

    }
    

    // get time 1 information

    result_recv = recv(cs, &avd, sizeof(int), 0);

    if (result_recv < 0){
        printf("ERROR GET TIME INFO\n");
        return -1;
    }

    else{

        //number = convert_str_to_int(avd);
        printf("TIME GOT IT: %d\n", number);

        // func to write time (need translate code htons or htonl)

        // func to send ok
        /* if (send_ok(cs) == -1){
            printf("ERROR SENDOK FUNC\n");
            return -1;
        } */

    }

    // get time 2 information

    result_recv = recv(cs, int_recv_buffer, sizeof(int), 0);

    if (result_recv < 0){
        printf("ERROR TIME 2 INFO\n");
        return -1;
    }

    else{

        number = convert_str_to_int(int_recv_buffer);
        printf("TIME 2 GOT IT: %d\n", number);

        // func to write time 2 (need translate code htons or htonl)

        // func to send ok
        /* if (send_ok(cs) == -1){
            printf("ERROR SENDOK FUNC\n");
            return -1;
        } */

    }

    // lendata 

    result_recv = recv(cs, int_recv_buffer, sizeof(int), 0);

    
    int lenData = convert_str_to_int(int_recv_buffer);

    if (result_recv < 0){
        printf("ERROR LEN MSG\n");
        return -1;
    }

    else{
        printf("LEN MSG GOT IT: %d\n", lenData);

        // func to write len message or create value with need buffer;


        // func to send ok
        /* if (send_ok(cs) == -1){
            printf("ERROR SENDOK FUNC\n");
            return -1;
        } */

    }


    // message
    
    lenData = 10;


    char *buffer_message = (char*)malloc(sizeof(char) * lenData + 1);

    result_recv = recv(cs, buffer_message, sizeof(char) * lenData, 0);
    printf("MESSAGE GOT IT: %d bytes\n", result_recv);
    printf("%s\n", buffer_message);






    close(cs);
    //printf("SMT for commit!");
    //printf("%s\n", int_recv_buffer);


    //result_recv = recv(cs, );


    
    //s_close(cs);
    // well 
    return 1;
    
    



    // send number message


}


int main()
{

    int connectSocket = -1;
    struct sockaddr_in addr = {0};
    int array_connectSockets[N] = {-1};
    fd_set rfd;
    fd_set wfd;
    int maxDescriptor = connectSocket;
    struct timeval timeValue = {1, 0};
    int count_arraySockets = 0;

    connectSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (connectSocket < 0){
        printf("ERROR SOCKET CONNECT\n");
        return -1;
    }
    
    set_non_block_mode(connectSocket);


    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    //addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int result_bind = bind(connectSocket, (struct sockaddr*) &addr, sizeof(addr));

    if (result_bind < 0){
        printf("ERROR BIND SOCKET TO ADDRESS\n");
        return -1;
    }

    if (listen(connectSocket, 1) < 0){
        printf("ERROR LISTEN SOCKET\n");
        return -1;
    }

    while(1) {

        // select code 
        FD_ZERO(&rfd);
        FD_ZERO(&wfd);

        FD_SET(connectSocket, &rfd);
        

        maxDescriptor = connectSocket;  // Сброс maxDescriptor перед каждым вызовом select
        for (int i = 0; i < N; i++) {
            if (array_connectSockets[i] > 0) {
                FD_SET(array_connectSockets[i], &rfd);
                FD_SET(array_connectSockets[i], &wfd);
                if (maxDescriptor < array_connectSockets[i])
                    maxDescriptor = array_connectSockets[i];
            }
        }

        
        int result_select = select(maxDescriptor + 1, &rfd, &wfd, 0, &timeValue);

        if (result_select > 0){
            //printf("<<<<<<<<<<<<<\n");
            //printf("%d\n", result_select);
            if (FD_ISSET(connectSocket, &rfd)){
                // accept func
                socklen_t addrlen = sizeof(addr);
                int cs = accept(connectSocket, (struct sockaddr*) &addr, &addrlen);
                printf("Numbers of sockets: %d\n", cs);
                if (cs < 0){
                    printf("ERROR NOT ACCEPT FUNC\n");
                }

                // mb error, need to write log
                //set_non_block_mode(cs);


                printf("<<<get it accept\n");
                

                // add new socket to array_connectSockets
                array_connectSockets[count_arraySockets] = cs;
                count_arraySockets++;


            }

            for (int i = 0; i < N; i++){
                if ((array_connectSockets[i] > 0) && (FD_ISSET(array_connectSockets[i], &rfd))){
                    // socket ready to read
                    printf("ready to read\n");
                    get_information_from_client(array_connectSockets[i]);

                }

                if ((array_connectSockets[i] > 0) && FD_ISSET(array_connectSockets[i], &wfd)){
                    // socket ready to write
                    printf("ready to write\n");
                    get_information_from_client(array_connectSockets[i]);

                }
            }

        }
    //else {

     //   printf("ERROR WHILE!\n");
    //}
        

        //socklen_t addrlen = sizeof(addr);
        
        // int cs = accept(connectSocket, (struct sockaddr*) &addr, &sizeof(addr));
        // unsigned int ip_client;

        // printf("1234\n");

        // if (cs < 0){
        //     printf("ERROR ACCEPT CONNECT CLIENT\n");
        //     return -1;
        // }

        // printf(">>WARNING\n");
        // ip_client = ntohl(addr.sin_addr.s_addr);
        // printf(" Client connected: %u.%u.%u.%u ", 
        //     (ip_client >> 24) & 0xFF, (ip_client >> 16) & 0xFF, (ip_client >> 8) & 0xFF, (ip_client) & 0xFF);

        // // GEt information from tcp_client

        // get_information_from_client(cs);

        // func to get string


        // func to send "ok"

        // s_close(cs);
        

    }


    



    
}