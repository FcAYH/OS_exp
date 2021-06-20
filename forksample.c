#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void Solve()
{
    pid_t pid1;
    if (pid1 = fork())
    {
        printf("The Father's pid is: %d\n", getpid());
    }
    else
    {
        printf("The child's pid is: %d;", getpid());
        printf(" and his Father's pid is: %d\n", getppid());
    }

    sleep(5);
}

int main()
{
    Solve();

    return 0;
}
