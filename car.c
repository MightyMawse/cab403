#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include "shared.h"
#include "tcp.h"

// CAR {name} {lowest floor} {highest floor}
// STATUS {current floor} {target floor} {status}

const char* car_name;

int main(int argc, char *argv[])
{
    /*
    if(argc != 4){
        printf("Usage: ./car {name} {delay} {lowest floor} {highest floor}");
        return 0;
    }
    */

    int fd = shm_open("/my_shm", O_RDWR, 0666);
    car_shared_mem *shm = mmap(NULL, sizeof(car_shared_mem), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    pthread_mutex_lock(&shm->mutex);

    return 0;
}