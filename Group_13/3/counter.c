#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   //fork
#include <sys/wait.h> //waitpid
#include <errno.h>

pid_t mainszalertek = 0;
int count_parent = 0;
int count_child = 0;

pid_t child1()
{
    pid_t szal = fork();
    if (szal == -1)
        exit(-1);
    if (szal > 0)
    {
        return szal;
    }
    printf("This is a Child Process!\n");
    while (count_child < 10)
    {
        printf("Child Process: %i\n", count_child);
        sleep(1);
        count_child++;
    }
    exit(0);
}

int main()
{
    mainszalertek = getpid();
    child1();
    int count_parent = 0;
    while (count_parent < 20)
    {
        printf("Parent Process: %i\n", count_parent);
        sleep(1);
        count_parent++;
    }

    wait(NULL); // wait(NULL) will only wait until the child process is completed. But, wait(&status) will return the process id of the child process that is terminated.

    return 0;
}