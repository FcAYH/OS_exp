#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

#define MaxN 4
#define waitTime 5000
#define worktime 996

sem_t occupied;
sem_t unoccupied;

pthread_mutex_t mutex;

int buffer[MaxN];
int putIn, takeOut;
int hasProduced = 0;

void delay(int x)
{
    int i = rand() % x;
    while (i)
    {
        int temp = rand() % x;
        while (temp)
            temp--;
        i--;
    }
}

void Producer()
{
    while (1)
    {
        int pName = rand() % 200 + 1;
        hasProduced++;

        delay(waitTime);
        // sleep(1);

        sem_wait(&unoccupied);
        pthread_mutex_lock(&mutex);

        buffer[putIn] = pName;
        printf("Put %d product into buffer at %d.\n", pName, putIn);
        putIn++;

        (putIn == MaxN) ? putIn = 0 : putIn;

        sem_post(&occupied);
        pthread_mutex_unlock(&mutex);

        if (hasProduced == worktime)
            return;
    }
}

void Consumer()
{
    while (1)
    {
        delay(waitTime);
        // sleep(1);

        sem_wait(&occupied);
        pthread_mutex_lock(&mutex);

        printf("Take out %d from the buffer at %d.\n", buffer[takeOut], takeOut);
        buffer[takeOut++] = -1;

        (takeOut == MaxN) ? takeOut = 0 : takeOut;

        sem_post(&unoccupied);
        pthread_mutex_unlock(&mutex);

        if (hasProduced == worktime)
            return;
    }
}

void Solve()
{
    srand(time(0));

    pthread_t manufacturer;
    pthread_t customer;

    sem_init(&occupied, 0, 0);
    sem_init(&unoccupied, 0, MaxN);

    pthread_mutex_init(&mutex, NULL);

    pthread_create(&manufacturer, NULL, (void *)Producer, NULL);
    pthread_create(&customer, NULL, (void *)Consumer, NULL);

    pthread_join(manufacturer, NULL);
    pthread_join(customer, NULL);

    sem_destroy(&unoccupied);
    sem_destroy(&occupied);
}

int main()
{
    Solve();

    return 0;
}