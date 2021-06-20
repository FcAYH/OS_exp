#pragma once

#define MaxSize 2048

typedef struct shmdata
{
    int written;
    char data[MaxSize];
} shmdata;