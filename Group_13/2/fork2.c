#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   //fork
#include <sys/wait.h> //waitpid
#include <errno.h>

pid_t mainszalertek = 0;
int notacommonvalue = 1;

pid_t child()
{
    // a return re szülségünk van
    // mert a fork() a gyereknek 0 val tér vissza, a szülőnek pedig a gyerek pid azonosítójával.

    // fork() returns 0 to the child process and the child's PID to the parent process.
    pid_t szal = fork();
    if (szal == -1)
        exit(-1);
    if (szal > 0)
    {
        return szal; // need that return here.
    }
    notacommonvalue = 5; //it changes the value of the copy of the variable
    printf("The value is %i in child process \n", notacommonvalue);
    // This is the important piece, the child branch must exit.
    //The critical thing is once the child is done doing whatever
    //it's doing the child branch must exit else it will continue along and run the rest of the code.
    exit(0);
}

int main()
{
    notacommonvalue = 2;
    printf("The value is %i before forking \n", notacommonvalue);

    mainszalertek = getpid();
    child();
    wait(NULL);
    printf("The value is %i in parent process (remain the original) \n", notacommonvalue);

    return 0;
}