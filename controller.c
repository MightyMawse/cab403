#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "shared.h"
#include "tcp.h"

#define SA struct sockaddr
int sockfd; 
int connfd;

#define STR_ACCEPTED "ACCEPTED" 
#define STR_UNAVAILABLE "UNAVAILABLE"

void tcp_server_listen( int connfd );
int tcp_server_init();
int shm_init();

int main()
{   
    if(tcp_server_init()){
        tcp_server_listen(connfd);
    }

    // After chatting close the socket
    close(sockfd);
}

int shm_init(){
    // creator (process A)
    int fd = shm_open("/my_shm", O_CREAT | O_RDWR, 0666);
    ftruncate(fd, sizeof(car_shared_mem));
    car_shared_mem *shm = mmap(NULL, sizeof(car_shared_mem), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    // init the mutex/cond as process-shared (creator only, once)
    pthread_mutexattr_t ma;
    pthread_mutexattr_init(&ma);
    pthread_mutexattr_setpshared(&ma, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&shm->mutex, &ma);

    pthread_condattr_t ca;
    pthread_condattr_init(&ca);
    pthread_condattr_setpshared(&ca, PTHREAD_PROCESS_SHARED);
    pthread_cond_init(&shm->cond, &ca);
}

int tcp_server_init(){
    int len;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, 1, 1);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
    {
        perror("bind");
        exit(1);
    }
    else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);

    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA *)&cli, &len);
    if (connfd < 0)
    {
        printf("server accept failed...\n");
        exit(0);
    }
    else
        printf("server accept the client...\n");

    return 1;
}

void tcp_server_listen( int connfd )
{
    uint8_t msg_buf[MAX_TCP_LENGTH];
    uint8_t* pbuf = msg_buf;

    read(connfd, msg_buf, sizeof(msg_buf));

    uint16_t msg_len = tcp_get_header_length(pbuf);

    if (msg_len > 0)
    {
        // Get cmd number
        uint8_t cmd = tcp_get_cmd(pbuf);

        switch (cmd)
        {
        case TCP_CMD_CALL:
            tcp_call_t call;

            tcp_deserialise_call(pbuf, &call);

            /* TODO */

            bzero(msg_buf, MAX_TCP_LENGTH);

            tcp_string_t string;
            string.str = STR_ACCEPTED;
            string.strlen = strlen(string.str);
            string.command = TCP_CMD_STRING;

            tcp_serialise_string(msg_buf, &string);
            write(connfd, msg_buf, sizeof(msg_buf));
            break;
        case TCP_CMD_CAR:
            break;
        case TCP_CMD_EMERGENCY:
            break;
        case TCP_CMD_FLOOR:
            break;
        case TCP_CMD_SELECT:
            break;
        case TCP_CMD_SERVICE:
            break;
        case TCP_CMD_STATUS:
            break;
        default:
            break;
        }

        close(connfd);
    }
}
