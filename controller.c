#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "shared.h"

#define SA struct sockaddr

#define AVAILABLE 0x01
#define UNAVAILABLE 0x02

void tcp_server_listen( int connfd );

int main()
{
    int sockfd, connfd, len;
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

    // Function for chatting between client and server

    tcp_server_listen(connfd);

    // After chatting close the socket
    close(sockfd);
}

void tcp_server_listen( int connfd )
{
    uint8_t msg_buf[MAX_TCP_LENGTH];
    uint8_t* pbuf;

    // read the message from client and copy it in buffer
    read(connfd, msg_buf, sizeof(msg_buf));

    uint16_t net_msg_len;
    uint16_t msg_len;

    pbuf = msg_buf;

    memcpy(&net_msg_len, pbuf, 2);
    pbuf += 2;
    msg_len = ntohs(net_msg_len);

    if (msg_len > 0)
    {
        // Get cmd number
        uint8_t cmd;
        memcpy(&cmd, pbuf, 1);
        pbuf++;

        switch (cmd)
        {
        case TCP_CMD_CALL:
            tcp_call_t *call = tcp_deserialise_call(pbuf);

            /* Test Deserialiser */
            printf("TCP_CMD_CALL\n");
            printf("%d\n", call->command);
            printf("%d\n", call->nmlen);
            printf("%s\n", call->nm);
            printf("%d\n", call->fl);
            printf("%d\n", call->ud);

            /* TODO */

            bzero(msg_buf, MAX_TCP_LENGTH);

            tcp_string_t string;
            string.str = "RESPONSE STRING";

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
