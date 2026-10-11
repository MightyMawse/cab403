#pragma once
#include <pthread.h>

// TCP Commands
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
  uint8_t tar_fl;
  uint8_t status;
} tcp_status_t;

typedef struct
{
  uint8_t command;
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
  const char *str;
} tcp_string_t;

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
tcp_call_t *tcp_deserialise_call( uint8_t *buf );
tcp_call_t *tcp_deserialise_status( uint8_t *buf );
tcp_call_t *tcp_deserialise_car( uint8_t *buf );
tcp_call_t *tcp_deserialise_floor( uint8_t *buf );
tcp_call_t *tcp_deserialise_select( uint8_t *buf );
tcp_call_t *tcp_deserialise_service( uint8_t *buf );
tcp_call_t *tcp_deserialise_emergency( uint8_t *buf );
tcp_call_t *tcp_deserialise_string( uint8_t *buf );

typedef struct
{
  pthread_mutex_t mutex;          // Locked while the contents of the structure are being accessed/modified
  pthread_cond_t cond;            // Signalled when the contents of the structure change
  char status[8];                 // NUL-terminated C string indicating the elevator's status
  char current_floor[4];          // NUL-terminated C string in the range of "B99" to "B1" and "1" to "999"
  char target_floor[4];           // Controller's immediate movement target
  char floor_request[4];          // Passenger floor selection mailbox
  uint8_t open_button;            // 1 if open doors button is pressed, 0 otherwise
  uint8_t close_button;           // 1 if close doors button is pressed, 0 otherwise
  uint8_t floor_request_pending;  // 1 while floor_request awaits transmission
  uint8_t door_obstruction;       // 1 if obstruction detected, 0 otherwise
  uint8_t overload;               // 1 if overload detected
  uint8_t emergency_stop;         // 1 if emergency stop button has been pressed, 0 otherwise
  uint8_t service_mode;           // 1 while in individual service mode
  uint8_t emergency_mode;         // 0 if not in emergency mode, 1 if in emergency mode
  uint32_t safety_heartbeat;      // Sequence incremented by the safety process
} car_shared_mem;