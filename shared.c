#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "shared.h"

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
    return 0;
}

int tcp_serialise_car(uint8_t *buf, const tcp_car_t* car)
{
    return 0;
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
    // Format
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

    *pbuf++ = TCP_CMD_STRING;

    len_buf = htons(str_len);
    memcpy(pbuf, &len_buf, 2);
    pbuf += 2;

    memcpy(pbuf, string->str, str_len);
    pbuf += str_len;

    return (int)total_msg_len;
}


// Deserialise (byte string -> struct)
tcp_call_t *tcp_deserialise_call(uint8_t *buf){
    // len cmd nmlen nm   fl u/d
    // 0000 00 0000 0...0 00 00

    // buf should already be set to nmlen position

    tcp_call_t call;

    uint8_t* pbuf = buf;
    uint8_t net_nmlen;
    
    memcpy(net_nmlen, buf, 2); //nmlen
    call.nmlen = ntohs(net_nmlen);
    buf += 2;

    memcpy(call.nm, buf, call.nmlen); // nm
    buf += call.nmlen;

    memcpy(call.fl, buf, 1); //fl
    buf++;

    memcpy(call.ud, buf, 1); //ud
    buf++;

    call.command = TCP_CMD_CALL;

    return &call;
}

tcp_call_t *tcp_deserialise_status(uint8_t *buf){
    return NULL;
}

tcp_call_t *tcp_deserialise_car(uint8_t *buf){
    return NULL;
}

tcp_call_t *tcp_deserialise_floor(uint8_t *buf){
    return NULL;
}

tcp_call_t *tcp_deserialise_select(uint8_t *buf){
    return NULL;
}

tcp_call_t *tcp_deserialise_service(uint8_t *buf){
    return NULL;
}

tcp_call_t *tcp_deserialise_emergency(uint8_t *buf){
    return NULL;
}

tcp_call_t *tcp_deserialise_string(uint8_t *buf){
    return NULL;
}