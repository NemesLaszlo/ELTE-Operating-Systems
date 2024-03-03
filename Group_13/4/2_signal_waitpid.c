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

    // exit(-1); -> Terminated with status 65280 -> 
    // a process with a status of 65280 likely exited normally but may have encountered an issue during execution that prompted it to terminate abruptly due to a signal

    // More:
    // A process with a status of 65280 typically indicates that the process has exited with an error. 
    // In Unix-like operating systems, including Linux, when a process exits, its exit status is represented as a 16-bit value.
    // The lower 8 bits represent the exit status of the process, while the upper 8 bits represent the signal that caused the process to terminate (if any).

    // A status of 65280 in decimal is equivalent to 0xFF00 in hexadecimal.
    // - The lower 8 bits are all zeros, indicating that the process exited without an error status.
    // - The upper 8 bits are 0xFF, which indicates that the process exited due to being terminated by a signal. 
    //   In this case, it's often SIGABRT (Signal Abort), which typically occurs when a process calls the abort() function to terminate itself due to a critical error.

    // exit(1); -> Error code 256 typically indicates a command or process exiting with a non-zero status, suggesting some kind of failure or error during execution. 
}

int main(int argc, char **argv)
{
    mainProcessValue = getpid();
    signal(SIGUSR1, readyHandler);

    pid_t child1_pid = child1();
    pid_t child2_pid = child2();

    while (ready < 1)
        ;
    puts("Child1 ready!");
    while (ready < 2)
        ;
    puts("Child2 ready!");

    int status;
    waitpid(child1_pid, &status, 0);
    printf("Child 1 terminated with status: %d\n", status);
    
    waitpid(child2_pid, &status, 0);
    printf("Child 2 terminated with status: %d\n", status);

    printf("Done\n");
    return 0;
}