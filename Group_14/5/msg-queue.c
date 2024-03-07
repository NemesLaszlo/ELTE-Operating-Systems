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

/*
msgget(): either returns the message queue identifier for a newly created message 
queue or returns the identifiers for a queue which exists with the same key value.

msgsnd(): Data is placed on to a message queue by calling msgsnd().

msgrcv(): messages are retrieved from a queue.

msgctl(): It performs various operations on a queue. Generally it is use to 
destroy message queue.
*/

struct message
{
    long mtype; //ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
    char mtext[1024];
};

pid_t mainProcessValue = 0;
int messageQueue;
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

    int status;
    struct message ms = {5, "This is a test sentance."};
    // msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
    // The third argument, msgsz, is the size of message (the message should end with a null character)
    status = msgsnd(messageQueue, &ms, strlen(ms.mtext) + 1, 0);
    // a 3. param ilyen is lehet: sizeof(uz.mtext)
    // a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
    /*
    IPC_NOWAIT röviden magyarul:
        Ha az üzenetsor megtelt, akkor az üzenetet nem írja a sorba, 
        és a vezérlés visszatér a hívási folyamathoz. Ha nincs megadva, 
        akkor a hívás folyamatát felfüggeszti (blokkolja), amíg az üzenet meg nem írható.
    */

    /*
    The argument msgflg specifies the action to be taken if one or more of the following are true:	 
        Placing the message on the message queue would cause the current number of bytes on the message queue (msg_cbytes) to exceed the maximum number of bytes allowed on this queue, as specified in msg_qbytes.
        The total number of messages on the queue is equal to the system-imposed limit.
    
    These actions are as follows:
    If the IPC_NOWAIT flag is on in msgflg, the message will not be sent and the calling process will return immediately. msgsnd() will return -1 and set errno to EAGAIN.
    If the IPC_NOWAIT flag is off in msgflg, the calling process will suspend execution until one of the following occurs:
        The condition responsible for the suspension no longer exists, in which case the message is sent.
        The message queue identifier, msgid, is removed from the system; when this occurs, errno is set to EIDRM and a value of -1 is returned.
        The calling process receives a signal that is to be caught; in this case a message is not sent and the calling process resumes execution. A value of -1 is returned and error is set to EINTR.
    */
    if (status < 0)
    {
        perror("msgsnd");
    }

    exit(0);
}

int main(int argc, char **argv)
{
    mainProcessValue = getpid();
    signal(SIGUSR1, readyHandler);

    int status;
    key_t mainKey;

    // msgget needs a key, amelyet az ftok generál
    // ftok - convert a pathname and a project identifier to a System V
    mainKey = ftok(argv[0], 1);
    printf("Key: %d\n", mainKey);
    // 0600 represents read and write permissions for the owner of the message queue.
    // IPC_CREAT is a flag that indicates that the message queue should be created if it does not already exist. 
    messageQueue = msgget(mainKey, 0600 | IPC_CREAT);
    if (messageQueue < 0)
    {
        perror("msgget");
        return 1;
    }

    child1();

    while (ready < 1)
        ;
    puts("Child1 kész!");

    struct message ms;
    //msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
    status = msgrcv(messageQueue, &ms, 1024, 5, 0);
    if (status < 0)
    {
        perror("msgrcv");
    }
    else
    {
        printf("A kapott üzenet a child1-től kodja: %ld, szovege: %s \n", ms.mtype, ms.mtext);
    }

    wait(NULL);
    status = msgctl(messageQueue, IPC_RMID, NULL);
    if (status < 0)
    {
        perror("msgctl");
    }
    return 0;
}