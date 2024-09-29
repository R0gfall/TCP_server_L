#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N 10
#define PORT 9001
#define GET_FLAG 1
#define PUT_FLAG 2
#define STOP_FLAG 1

int PG_OPERATION = 0;
FILE* fName;


void get_client_info(int clientSocket);

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

    send_bytes = send(cs, ok_msg, 2, 0);
    if (send_bytes < 0){
        printf("ERROR SEND OK MSG\n");
        return -1;
    }
    
    printf("OK MSG SEND %s, %d bytes\n", ok_msg, send_bytes);
    return 1;
}


void convert_date(uint32_t number, char *date_str) {
    // Извлекаем день, месяц и год
    uint32_t day = number % 100;
    uint32_t month = (number / 100) % 100;
    uint32_t year = number / 10000;

    // Форматируем строку как "dd.mm.yyyy"
    sprintf(date_str, "%02u.%02u.%04u", day, month, year);
}


void convert_time(uint32_t number, char *time_str) {
    // Извлекаем секунды, минуты и часы
    uint32_t seconds = number % 100;
    uint32_t minutes = (number / 100) % 100;
    uint32_t hours = number / 10000;

    // Форматируем строку как "hh:mm:ss"
    sprintf(time_str, "%02u:%02u:%02u", hours, minutes, seconds);
}


int first_put_or_get(int cs)
{   
    char* recv_buffer_PG = (char*)malloc(sizeof(char) * 3);
    int result_recv;

    result_recv = recv(cs, recv_buffer_PG, 3, 0);
    if (result_recv < 0){
        printf("ERROR: PUT, GET\n");
        return -1;
    }

    if (strcmp(recv_buffer_PG, "put") == 0){
        printf("PUT IS ALL READY %s, %d bytes\n", recv_buffer_PG, result_recv);
        return PUT_FLAG;
    }

    else if (strcmp(recv_buffer_PG, "get") == 0){
        printf("GET IS ALL READY %s, %d bytes\n", recv_buffer_PG, result_recv);
        return GET_FLAG;
    }

    printf("INCORRECT FB MSG\n");
    
    return -1;

}

int stop_word(int cs, char* buffer, int lenData)
{
    
    buffer[lenData] = '\0';
    if (strcmp(buffer, "stop") == 0){
        printf("FIND STOP: %s\n", buffer);
        return STOP_FLAG; 
    }

    return -1;
}

int get_information_from_client(int cs)
{
    
    // maybe need to change char to integer

    get_client_info(cs);


    printf("PG_OPERATION: %d\n",PG_OPERATION);
    if (PG_OPERATION == 0){
        PG_OPERATION++;
        int res = first_put_or_get(cs);
        if (res < 0){
            return -1;
        }
    }



    char* int_recv_buffer = (char*)malloc(sizeof(int)); // or int* int_recv_buffer = (int*)malloc(sizeof(int));


    int result_recv, number;

    char date_str[11], time_str[9];

    // get number message

    uint32_t avd;

    result_recv = recv(cs, &avd, sizeof(int), 0);

    if (result_recv <= 0){
        printf("ERROR NUMBER MESSAGE: %d bytes\n", result_recv);
        return -1;
    }
    else{

        printf("NUMBER OF MESSAGE GOT IT: %d, %d bytes\n", ntohl(avd), result_recv);
        fprintf(fName, "%d ", ntohl(avd));

    }


    // get date information

    result_recv = recv(cs, &avd, sizeof(int), 0);

    //printf("%d\n", avd);
    if (result_recv <= 0){
        printf("ERROR GET DATE INFO\n");
        return -1;
    }

    else{

        
        convert_date(ntohl(avd), date_str);

        //printf(">>>%d, !!!%s\n", result_recv, date_str);
        //number = convert_str_to_int(avd);

        printf("DATE GOT IT: %s, %d bytes\n", date_str, result_recv);

        fprintf(fName, "%s ", date_str);
        // func to write data (need translate code htons or htonl)


    }
    

    // get time 1 information

    result_recv = recv(cs, &avd, sizeof(int), 0);

    if (result_recv < 0){
        printf("ERROR GET TIME INFO\n");
        return -1;
    }

    else{

        //number = convert_str_to_int(avd);
        convert_time(ntohl(avd), time_str);

        printf("TIME GOT IT: %s, %d bytes\n", time_str, result_recv);

        fprintf(fName, "%s ", time_str);
        // func to write time (need translate code htons or htonl)


    }

    // get time 2 information

    result_recv = recv(cs, &avd, sizeof(int), 0);

    if (result_recv <= 0){
        printf("ERROR TIME 2 INFO\n");
        return -1;
    }

    else{

        convert_time(ntohl(avd), time_str);

        printf("TIME GOT IT: %s, %d bytes\n", time_str, result_recv);

        fprintf(fName, "%s ", time_str);
        // func to write time 2 (need translate code htons or htonl)
    }

    // lendata 

    result_recv = recv(cs, &avd, sizeof(int), 0);

    
    int lenData = ntohl(avd);

    if (result_recv <= 0){
        printf("ERROR LEN MSG\n");
        return -1;
    }

    else{
        printf("LEN MSG GOT IT: %d, %d bytes\n", lenData, result_recv);
        // func to write len message or create value with need buffer;
        fprintf(fName, "%d ", lenData);
    }


    // message

    //>>>>>>
    // char *buffer_message = (char*)malloc(sizeof(char) * lenData);

    // result_recv = recv(cs, buffer_message, sizeof(char) * lenData, 0);
    // printf("MESSAGE GOT IT: %d bytes\n", result_recv);
    // printf("%s\n", buffer_message);

    //>>>>>>

    //----


    char *buffer_message = (char*)malloc(sizeof(char) * lenData);
    int rcv, curlen = 0;
    do
    {
        int i;
        rcv = recv(cs, buffer_message, sizeof(buffer_message), 0);
        for (i = 0; i < rcv; i++)
        {
            if (buffer_message[i] == '\n')
            return curlen;
            curlen++;
        }
        if (curlen > 500000)
        {
            printf("input string too large\n");
            return 500000;
        }

    } while (rcv > 0);
    //----



    //fprintf(fName, "%s\n", buffer_message);
    //buffer_message + lenData = '\0';
    
    if (send_ok(cs) == -1){
        printf("ERROR SENDOK FUNC\n");
        return -1;
    } 


    strcat(buffer_message, "'\0'");
    if (stop_word(cs, buffer_message, lenData) == 1){
        fprintf(fName, "%s\n", buffer_message);
        return -1;
    }
    fprintf(fName, "%s\n", buffer_message);

    printf("\n");
    free(int_recv_buffer);
    free(buffer_message);

    return 1;

}


void get_client_info(int client_socket) {
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // Получение информации о подключенном клиенте
    if (getpeername(client_socket, (struct sockaddr*)&client_addr, &addr_len) == -1) {
        perror("getpeername");
        return;
    }

    // Извлечение IP-адреса и порта
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
    int client_port = ntohs(client_addr.sin_port);

    printf("Client IP: %s\n", client_ip);
    printf("Client Port: %d\n", client_port);
    fprintf(fName, "%s %d", client_ip, client_port);
    
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
    fName = fopen("msg.txt", "w");

    connectSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (connectSocket < 0){
        printf("ERROR SOCKET CONNECT\n");
        return -1;
    }
    
    set_non_block_mode(connectSocket);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);;

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
                //FD_SET(array_connectSockets[i], &wfd);
                if (maxDescriptor < array_connectSockets[i])
                    maxDescriptor = array_connectSockets[i];
            }
        }

        
        int result_select = select(maxDescriptor + 1, &rfd, &wfd, 0, &timeValue);

        if (result_select > 0){
            
            if (FD_ISSET(connectSocket, &rfd)){
                // accept func
                socklen_t addrlen = sizeof(addr);
                int cs = accept(connectSocket, (struct sockaddr*) &addr, &addrlen);
                printf("Numbers of sockets: %d\n", cs);
                if (cs < 0){
                    printf("ERROR NOT ACCEPT FUNC\n");
                }
                
                //get_client_info(cs);

                // mb error, need to write log
                set_non_block_mode(cs);


                printf("<<<get it accept\n");
                
                //maxDescriptor = connectSocket; 
                // add new socket to array_connectSockets
                array_connectSockets[count_arraySockets] = cs;
                count_arraySockets++;


            }

            for (int i = 0; i < count_arraySockets; i++){
                if ((array_connectSockets[i] > 0) && (FD_ISSET(array_connectSockets[i], &rfd))){
                    // socket ready to read
                    printf("<<<%d\n", count_arraySockets);
                    printf("ready to read: %d\n", i);



                    int result_get = get_information_from_client(array_connectSockets[i]);
                    if (result_get == -1){
                        close(array_connectSockets[i]);
                        printf("CLOSE SOCKET AND TCP CONNECT\n");
                        fclose(fName);
                        FD_CLR(array_connectSockets[i], &rfd);
                        close(connectSocket);
                        //FD_CLR(array_connectSockets[i], &wfd);
                    }
                    //printf(">%d\n", result_get);
                    // FD_CLR(array_connectSockets[i], &rfd);
                    // FD_CLR(array_connectSockets[i], &wfd);

                }

                if ((array_connectSockets[i] > 0) && FD_ISSET(array_connectSockets[i], &wfd)){
                    // socket ready to write
                    //printf("ready to write\n");
                    //get_information_from_client(array_connectSockets[i]);
                    /* if (send_ok(array_connectSockets[i]) == -1){
                        printf("ERROR SENDOK FUNC\n");
                        return -1;
                    } */


                }
            }

        }
        else{
            //printf("ERROR SELECT >><<\n");

        }

    }


    



    
}