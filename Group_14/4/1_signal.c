#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

/*
A signal is a software generated interrupt that is sent to a process by
the OS because of when user press ctrl-c (SIGINT) or another process tell something to this process.
There are fix set of signals that can be sent to a process. signal are identified by integers.
Signal number have symbolic names. For example SIGCHLD is number of the signal sent to the parent process when child terminates.
*/

/*
A SIGTERM jel egy általános jel, amelyet a program lezárásához vezetnek. 
A SIGKILL-től eltérően ez a jel (SIGTERM) blokkolható, kezelhető és figyelmen kívül hagyható. 
A program megszüntetésének udvarias kérése, módja.
SIGINT - Interrupt the process
*/

/*
 SIGTERM -> 15
 SIGKILL -> 9

 pl.
 kill -15 "pidje a processnek amit ps -aux al kérdezel le a felhasználódra""

*/

void handler(int signumber)
{
    printf("My signal handler %i\n", signumber);
}

void handler_sigkill(int signumber)
{
    printf("My signal handler %i\n", signumber);
}

int main()
{

    int i = 0;
    signal(SIGTERM, handler);
    signal(SIGKILL, handler_sigkill);

    while (1)
    {
        printf("i = %i\n", i);
        i++;
        sleep(1);
    }
    return 0;
}