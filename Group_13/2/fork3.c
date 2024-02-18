#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   //fork
#include <sys/wait.h> //waitpid
#include <errno.h>

int notacommonvalue = 1;
pid_t mainszalertek = 0;

pid_t child1()
{
    pid_t szal = fork();
    if (szal == -1)
        exit(-1);
    if (szal > 0)
    {
        return szal;
    }
    notacommonvalue = 5;
    printf("The value is %i in child process \n", notacommonvalue);
    exit(0);
}

pid_t child2()
{
    pid_t szal = fork();
    if (szal == -1)
        exit(-1);
    if (szal > 0)
    {
        return szal;
    }
    notacommonvalue = 16;
    printf("The value is %i in child2 process \n", notacommonvalue);
    exit(0);
}

int main()
{
    notacommonvalue = 1;
    printf("The value is %i before forking \n", notacommonvalue);
    mainszalertek = getpid();

    child1();
    child2();
    wait(NULL); // wait(NULL) will only wait until the child process is completed. But, wait(&status) will return the process id of the child process that is terminated.
    printf("The value is %i in parent process (remain the original) \n", notacommonvalue);

    return 0;
}