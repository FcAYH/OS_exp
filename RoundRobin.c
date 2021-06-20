#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MaxTask 10
#define MaxTime 100

char record[MaxTime][3];            //记录答案
char RRoutput[MaxTask][MaxTime][3]; //用于输出答案

typedef struct Tasks // 进程
{
    char name[2];          //进程名称
    double come_time;      //到达时间
    double run_begin_time; //开始运行时间
    double run_time;       //运行时间
    double run_end_time;   //运行结束时间
    int run_flag;          //调度标志
    int start_flag;        //是否为第一次开始调度
} Tasks;

Tasks *tasks;         // 进程
int counter;          //实际进程个数
int time_counter = 0; //时间片长度

void ProcessInput() //进程参数输入
{
    printf("请输入进程数目:\n");
    scanf("%d", &counter);

    printf("请输入时间片长度:\n");
    scanf("%d", &time_counter);

    tasks = (Tasks *)malloc(sizeof(Tasks) * counter);

    int temp = 0;
    for (int i = 0; i < counter; i++)
    {
        printf("******************************************\n");
        printf("进程 %d: \n", i + 1);

        printf("进程名: ");
        scanf("%s", tasks[i].name);

        printf("进程到达时间: ");
        scanf("%lf", &tasks[i].come_time);

        printf("进程运行时间: ");
        scanf("%lf", &tasks[i].run_time);

        tasks[i].run_begin_time = 0;
        tasks[i].run_end_time = 0;
        tasks[i].run_flag = 0;   //运行是否结束
        tasks[i].start_flag = 0; //是否首次被执行
    }
}

int charge() //判断是否全部进程都执行完毕
{
    for (int i = 0; i < counter; i++)
        if (tasks[i].run_flag == 0)
            return 1;

    return 0;
}

int RoundRobin() //RooundRobin时间片调度
{
    for (int i = 0; i < MaxTime; i++)
        strcpy(record[i], "-");

    double time_temp = 0;

    Tasks *copy_task; //备份
    copy_task = (Tasks *)malloc(sizeof(Tasks) * counter);

    for (int i = 0; i < counter; i++)
    {
        copy_task[i] = tasks[i]; //对进程的初始化信息备份
    }

    time_temp = tasks[0].come_time;
    while (charge())
    {
        for (int i = 0; i < counter; i++)
        {
            if (tasks[i].come_time > time_temp)
            {
                time_temp = tasks[i].come_time;
            }

            if (tasks[i].run_flag == 0) //该进程还未结束
            {
                if (tasks[i].start_flag == 0) //该条件成立则说明，该进程是第一次执行，记录开始执行时间
                {
                    tasks[i].run_begin_time = time_temp;
                    tasks[i].start_flag = 1;
                }

                if (tasks[i].run_time / time_counter > 1) //程序仍需运行超过一个时间片
                {
                    tasks[i].run_time = tasks[i].run_time - time_counter;

                    for (int t = time_temp; t < time_temp + time_counter; t++)
                        strcpy(record[t], tasks[i].name);

                    time_temp = time_temp + time_counter;

                    if (tasks[i].run_time == 0)
                    {
                        tasks[i].run_end_time = time_temp;
                        tasks[i].run_flag = 1;
                        tasks[i].run_time = copy_task[i].run_time;
                    }
                }
                else //仅剩下一个时间片以内的运行时间
                {
                    for (int t = time_temp; t < time_temp + tasks[i].run_time; t++)
                        strcpy(record[t], tasks[i].name);

                    time_temp = time_temp + tasks[i].run_time;

                    tasks[i].run_end_time = time_temp;
                    tasks[i].run_flag = 1;
                    tasks[i].run_time = copy_task[i].run_time;
                }
            }
        }
    }

    return time_temp;
}

void Print(int allTime) //调度结果输出
{
    printf("\n时间片轮转算法。\n\n");

    for (int i = 0; i < counter; i++)
    {
        for (int j = 0; j <= allTime; j++)
            strcpy(RRoutput[i][j], "-");
    }
    for (int i = 0; i <= allTime; i++)
    {
        if (record[i][0] != '-')
        {
            for (int j = 0; j < counter; j++)
            {
                if (strcmp(tasks[j].name, record[i]) == 0)
                    strcpy(RRoutput[j][i], record[i]);
            }
        }
    }

    for (int i = 0; i < counter; i++)
    {
        printf("进程%d(%s): ", (i + 1), tasks[i].name);
        for (int j = 0; j <= allTime; j++)
            printf("%s", RRoutput[i][j]);
        puts("");
    }
    puts("");

    double turn_round_time = 0;
    printf("进程名 到达时间 运行时间 开始时间 结束时间 周转时间\n");
    for (int i = 0; i < counter; i++)
    {
        double turnover = tasks[i].run_end_time - tasks[i].come_time;
        turn_round_time += turnover;
        printf("%s\t%5.3lf\t%5.3lf\t %5.3lf\t  %5.3lf   %5.3lf\n", tasks[i].name, tasks[i].come_time, tasks[i].run_time, tasks[i].run_begin_time, tasks[i].run_end_time, turnover);
    }
    printf("平均周转时间 = %5.2lf\n", turn_round_time / counter);
}

void Solve()
{
    ProcessInput();

    int allTime = RoundRobin();

    Print(allTime);
}

int main()
{

    Solve();

    return 0;
}

/*

4
2
A 1 5 
B 1 9 
C 3 5 
D 6 10 
*/