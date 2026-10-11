#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "shared.h"

#define AVAILABLE 0x01
#define UNAVAILABLE 0x02

char *car_name;
uint8_t floor;
uint8_t direction;

int sock_fd;
struct sockaddr_in addr;

// ./call {car name} {floor} {up|down}
int main(int argc, char *argv[])
{
    if(argc != 4){
        printf("Usage: ./call {car name} {floor} {up|down}\n");
        return 0;
    }

    car_name = argv[1];
    if(strlen(car_name) == 0){
        printf("Invalid car name.\n");
        return 0;
    }

    floor = atoi(argv[2]);
    if(floor == 0){
        printf("Invalid floor specified.\n %s", argv[2]);
        return 0;
    }

    direction = argv[3] == "up" ? 1 : 2;
    if(direction == 0){
        printf("Invalid direction specified.\n");
        return 0;
    }

    // Open socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &addr.sin_addr) <= 0)
    {
        perror("inet_pton");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    if (connect(sock_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        printf("Unable to connect to elevator system.");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    uint8_t msg_buf[MAX_TCP_LENGTH];
    tcp_call_t call;

    call.command = TCP_CMD_CALL;
    call.fl = floor;
    call.nm = car_name;
    call.ud = direction;
    
    int byte_str_len = tcp_serialise_call(&msg_buf, &call);

    // Send response
    send(sock_fd, msg_buf, byte_str_len, 0);

    uint8_t resp_buf[MAX_TCP_LENGTH];
    ssize_t resp_size = read(sock_fd, resp_buf, sizeof(resp_buf));

    if(resp_size < 0){
        perror("read");
    }
    else if(resp_size == 0){
        printf("Unable to connect to elevator system.");
    }
    else{
        uint16_t net_msg_len;
        uint16_t msg_len;

        memcpy(&net_msg_len, resp_buf, 2);
        msg_len = ntohs(net_msg_len);

        for(int i = 0; i < msg_len; i++){
            printf("%c", resp_buf[i]);
        }
    }
}