#include "sys/types.h"
#include "unistd.h"
#include "stdlib.h"
#include "signal.h"
#include "stdio.h"
#include "string.h"
#include "time.h"
#include "wait.h"
#include "sys/ipc.h"
#include "sys/msg.h"
#include "sys/shm.h"
#include "sys/sem.h"
#include "sys/stat.h"

pid_t mainProcessValue = 0;
int ready = 0;

void readyHandler(int sig)
{
    if (sig == SIGUSR1)
    {
        ready++;
    }
}

pid_t child1()
{
    pid_t process = fork();
    if (process == -1)
        exit(-1);
    if (process > 0)
    {
        return process;
    }

    kill(mainProcessValue, SIGUSR1);

    exit(0);
}

pid_t child2()
{
    pid_t process = fork();
    if (process == -1)
        exit(-1);
    if (process > 0)
    {
        return process;
    }

    kill(mainProcessValue, SIGUSR1);

    exit(0);
}

int main(int argc, char **argv)
{
    mainProcessValue = getpid();
    signal(SIGUSR1, readyHandler);

    child1();
    child2();

    while (ready < 1)
        ;
    puts("Child1 ready!");
    while (ready < 2)
        ;
    puts("Child2 ready!");

    printf("Done\n");
  
    wait(NULL);
    return 0;
}