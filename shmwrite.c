#include "stdafx.h"

void Solve()
{
    int running = 1;
    void *shm = NULL;
    SharedData *shared = NULL;
    char buffer[BUFSIZ + 1]; //用于保存输入的文本
    int shmid;
    key_t key = ftok("read", 1);
    //创建共享内存
    shmid = shmget(key, sizeof(SharedData), 0666 | IPC_CREAT);
    if (shmid == -1)
    {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }
    //将共享内存连接到当前进程的地址空间
    shm = shmat(shmid, (void *)0, 0);
    if (shm == (void *)-1)
    {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Memory attached at %X\n", (int)shm);
    //设置共享内存
    shared = (SharedData *)shm;
    while (running) //向共享内存中写数据
    {
        //数据还没有被读取，则等待数据被读取,不能向共享内存中写入文本
        while (shared->written == 1)
        {
            sleep(1);
            printf("Waiting...\n");
        }
        //向共享内存中写入数据
        printf("Enter some text: ");
        fgets(buffer, BUFSIZ, stdin);
        strncpy(shared->data, buffer, TEXT_SZ);
        //写完数据，设置written使共享内存段可读
        shared->written = 1;
        //输入了end，退出循环（程序）
        if (strncmp(buffer, "end", 3) == 0)
            running = 0;
    }
    //把共享内存从当前进程中分离
    if (shmdt(shm) == -1)
    {
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }
    sleep(2);
    exit(EXIT_SUCCESS);
}

int main()
{
    Solve();

    return 0;
}