#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

int waitingFlag = 1;

void Stop1()
{
    printf("Interruption\n");
}
void Stop2()
{
    waitingFlag = 0;
    printf("Child Interruption\n");
}

void Solve()
{
    pid_t pid1, pid2;
    signal(SIGINT, Stop1);

    sleep(1);
    pid1 = fork();

    if (pid1 > 0)
    {
        pid2 = fork();

        if (pid2 > 0)
        {
            sleep(5);
            kill(pid1, SIGUSR1);
            wait(0);
            kill(pid2, SIGUSR2);
            wait(0);
            printf("Parent process is killed\n");
            //exit(0);
        }
        else
        {
            signal(SIGUSR2, Stop2);
            signal(SIGINT, SIG_IGN);

            while (waitingFlag)
                ;
            printf("Child process 2 is killed\n");
        }
    }
    else
    {
        signal(SIGUSR1, Stop2);
        signal(SIGINT, SIG_IGN);

        while (waitingFlag)
            ;
        printf("Child process 1 is killed\n");
    }
}

int main()
{
    Solve();

    return 0;
}
