#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// TCP Commands
#define TCP_CMD_UNDEFINED 0xFF
#define TCP_CMD_CALL 0x01
#define TCP_CMD_STATUS 0x02
#define TCP_CMD_CAR 0x03
#define TCP_CMD_FLOOR 0x04
#define TCP_CMD_SELECT 0x05
#define TCP_CMD_SERVICE 0x06
#define TCP_CMD_EMERGENCY 0x07
#define TCP_CMD_STRING 0x08
#define TCP_CMD_BYTESTR 0x09

// TCP Non-variable lengths
#define TCP_LENGTH_CALL 0x07
#define TCP_LENGTH_STR 0x04
#define TCP_LENGTH_CAR 0x07
#define TCP_LENGTH_STATUS 0x06

#define PORT 3001
#define SERVER_IP "127.0.0.1"

#define MAX_TCP_LENGTH 4096

typedef struct
{
  uint8_t command;
  uint16_t nmlen;
  const char *nm;
  uint8_t fl;
  uint8_t ud;
} tcp_call_t;

typedef struct
{
  uint8_t command;
  uint8_t cur_fl;
  uint8_t tar_fl;
  uint8_t status;
} tcp_status_t;

typedef struct
{
  uint8_t command;
  uint16_t nmlen;
  const char *nm;
  uint8_t low_fl;
  uint8_t high_fl;
} tcp_car_t;

typedef struct
{
  uint8_t command;
  uint8_t fl;
} tcp_floor_t;

typedef struct
{
  uint8_t command;
  uint8_t fl;
} tcp_select_t;

typedef struct
{
  uint8_t command;
} tcp_service_t;

typedef struct
{
  uint8_t command;
} tcp_emergency_t;

typedef struct
{
  uint8_t command;
  uint16_t strlen;
  const char *str;
} tcp_string_t;

// TCP Utils (Reads unofficial header)
uint8_t tcp_get_cmd( uint8_t *buf );
uint16_t tcp_get_header_length( uint8_t *buf );

// TCP Serialisers
int tcp_serialise_call( uint8_t *buf, const tcp_call_t* call );
int tcp_serialise_status( uint8_t *buf, const tcp_status_t* status );
int tcp_serialise_car( uint8_t *buf, const tcp_car_t* car );
int tcp_serialise_floor( uint8_t *buf, const tcp_floor_t* floor );
int tcp_serialise_select( uint8_t *buf, const tcp_select_t* select );
int tcp_serialise_service( uint8_t *buf, const tcp_service_t* service );
int tcp_serialise_emergency( uint8_t *buf, const tcp_emergency_t* emergency );
int tcp_serialise_string( uint8_t *buf, const tcp_string_t* string );

// TCP Deserialiserss
void tcp_deserialise_call( uint8_t *buf, tcp_call_t *call );
void tcp_deserialise_status( uint8_t *buf );
void tcp_deserialise_car( uint8_t *buf, tcp_car_t *car );
void tcp_deserialise_floor( uint8_t *buf );
void tcp_deserialise_select( uint8_t *buf );
void tcp_deserialise_service( uint8_t *buf );
void tcp_deserialise_emergency( uint8_t *buf );
void tcp_deserialise_string( uint8_t *buf, tcp_string_t* string );