#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "tcp.h"

// TCP Utils
uint8_t tcp_get_cmd(uint8_t *buf){
    uint8_t *pbuf = buf;
    uint8_t cmd;
    pbuf += 2; // Shift past length
    memcpy(&cmd, pbuf, 1);
    if(cmd <= 0){ return TCP_CMD_UNDEFINED; }
    return cmd;
}

uint16_t tcp_get_header_length(uint8_t *buf){
    uint8_t *pbuf = buf;
    uint16_t net_len, len;
    memcpy(&net_len, pbuf, 2);
    len = ntohs(net_len);
    return len;
}

// Serialise (struct -> byte string)
int tcp_serialise_call(uint8_t *buf, const tcp_call_t* call)
{
    // len cmd nmlen nm   fl u/d
    // 0000 00 0000 0...0 00 00

    uint16_t nmlen = strlen(call->nm);
    size_t total_msg_len = nmlen + TCP_LENGTH_CALL;
    if (total_msg_len > MAX_TCP_LENGTH)
        return -1;

    uint8_t *pbuf = buf;
    uint16_t len_buf;

    len_buf = htons(total_msg_len); // len
    memcpy(pbuf, &len_buf, 2);
    pbuf += 2;

    *pbuf++ = TCP_CMD_CALL; // cmd

    len_buf = htons(nmlen); // nmlen
    memcpy(pbuf, &len_buf, 2);
    pbuf += 2;

    memcpy(pbuf, call->nm, nmlen);
    pbuf += nmlen; // nm

    *pbuf++ = call->fl; // fl
    *pbuf++ = call->ud; // u/d

    return (int)total_msg_len;
}

int tcp_serialise_status(uint8_t *buf, const tcp_status_t* status)
{
    //STATUS {current floor} {target floor} {status}
    // 0000 00 00  00  00
    // len cmd cfl tfl status

    size_t total_msg_len = TCP_LENGTH_STATUS;
    if (total_msg_len > MAX_TCP_LENGTH)
        return -1;

    uint8_t* pbuf = buf;
    uint16_t len_buf;

    len_buf = htons(total_msg_len); // len
    memcpy(pbuf, &len_buf, 2);
    pbuf += 2;
    
    *pbuf++ = TCP_CMD_STATUS;
    *pbuf++ = status->cur_fl;
    *pbuf++ = status->tar_fl;
    *pbuf++ = status->status;

    return (int)total_msg_len;
}

int tcp_serialise_car(uint8_t *buf, const tcp_car_t* car)
{
    // 0000 00 0000  0...0 00 00
    // len cmd nmlen nm    lf hf

    uint16_t nmlen = strlen(car->nm);
    size_t total_msg_len = nmlen + TCP_LENGTH_CAR;
    if (total_msg_len > MAX_TCP_LENGTH)
        return -1;

    uint8_t *pbuf = buf;
    uint16_t len_buf;

    len_buf = htons(total_msg_len); // len
    memcpy(pbuf, &len_buf, 2);
    pbuf += 2;

    *pbuf++ = TCP_CMD_CAR; // cmd

    len_buf = htons(nmlen); // nmlen
    memcpy(pbuf, &len_buf, 2);
    pbuf += 2;

    memcpy(pbuf, car->nm, nmlen);
    pbuf += nmlen; // nm

    *pbuf++ = car->low_fl; // lf
    *pbuf++ = car->high_fl; // hf

    return (int)total_msg_len;
}

int tcp_serialise_floor(uint8_t *buf, const tcp_floor_t* floor)
{
    return 0;
}

int tcp_serialise_select(uint8_t *buf, const tcp_select_t* select)
{
    return 0;
}

int tcp_serialise_service(uint8_t *buf, const tcp_service_t* service)
{
    return 0;
}

int tcp_serialise_emergency(uint8_t *buf, const tcp_emergency_t* emergency)
{
    return 0;
}

int tcp_serialise_string(uint8_t *buf, const tcp_string_t* string){
    // len cmd strlen str
    // 0000 0   0000  0...0

    uint16_t str_len = strlen(string->str);
    uint16_t total_msg_len = TCP_LENGTH_STR + str_len;

    if (total_msg_len > MAX_TCP_LENGTH)
        return -1;

    uint8_t *pbuf = buf;
    uint16_t len_buf;

    len_buf = htons(total_msg_len); // len
    memcpy(pbuf, &len_buf, 2);
    pbuf += 2;

    *pbuf++ = TCP_CMD_STRING; // cmd

    len_buf = htons(str_len); // strlen
    memcpy(pbuf, &len_buf, 2);
    pbuf += 2;

    memcpy(pbuf, string->str, str_len); // str
    pbuf += str_len;

    return (int)total_msg_len;
}


// Deserialise (byte string -> struct)
void tcp_deserialise_call(uint8_t *buf, tcp_call_t* call){
    // len cmd nmlen nm   fl u/d
    // 0000 00 0000 0...0 00 00

    uint8_t* pbuf = buf;
    uint16_t net_nmlen;

    pbuf += 3; // Jump header
    
    memcpy(&net_nmlen, pbuf, 2); //nmlen
    call->nmlen = ntohs(net_nmlen);
    pbuf += 2;

    char* nm_buf = malloc(call->nmlen);
    memcpy(nm_buf, pbuf, call->nmlen); // nm
    call->nm = nm_buf;
    pbuf += call->nmlen;

    memcpy(&call->fl, pbuf, 1); //fl
    pbuf++;

    memcpy(&call->ud, pbuf, 1); //ud
    pbuf++;

    call->command = TCP_CMD_CALL;
}

void tcp_deserialise_status(uint8_t *buf){
    return NULL;
}

void tcp_deserialise_car(uint8_t *buf, tcp_car_t *car){
    // len cmd nmlen nm   lf hf
    // 0000 00 0000 0...0 00 00

    uint8_t* pbuf = buf;
    uint16_t net_nmlen;

    pbuf += 3; // Jump header
    
    memcpy(&net_nmlen, pbuf, 2); //nmlen
    car->nmlen = ntohs(net_nmlen);
    pbuf += 2;

    char* nm_buf = malloc(car->nmlen);
    memcpy(nm_buf, pbuf, car->nmlen); // nm
    car->nm = nm_buf;
    pbuf += car->nmlen;

    memcpy(&car->low_fl, pbuf, 1); //lf
    pbuf++;

    memcpy(&car->high_fl, pbuf, 1); //hf
    pbuf++;

    car->command = TCP_CMD_CAR;
}

void tcp_deserialise_floor(uint8_t *buf){
    return NULL;
}

void tcp_deserialise_select(uint8_t *buf){
    return NULL;
}

void tcp_deserialise_service(uint8_t *buf){
    return NULL;
}

void tcp_deserialise_emergency(uint8_t *buf){
    return NULL;
}

void tcp_deserialise_string(uint8_t *buf, tcp_string_t *string){
    // len cmd strlen str
    // 0000 0   0000  0...0
    uint8_t *pbuf = buf;

    pbuf += 3; // Jump header

    uint16_t net_strlen;
    uint16_t strlen;

    memcpy(&net_strlen, pbuf, 2); // strlen
    strlen = ntohs(net_strlen);
    string->strlen = strlen;
    pbuf += 2;

    char *str = malloc(strlen); // str
    memcpy(str, pbuf, strlen);
    string->str = str;
    pbuf += strlen;

    string->command = TCP_CMD_STRING; // cmd
}