#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct uzenet
{
    long mtype; //ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
    char mtext[1024];
};

struct uzenet2
{
    long mtype;
    int marr[5];
};

// sendig a message
int szulo(int uzenetsor)
{
    struct uzenet uz;
    struct uzenet2 uz2 = {5, {1, 2, 3, 4, 5}};
    int status;

    status = msgsnd(uzenetsor, &uz2, sizeof(uz2.marr), 0);
    if (status < 0)
        perror("msgsnd");

    status = msgrcv(uzenetsor, &uz, 1024, 4, 0);
    if (status < 0)
        perror("msgrcv");
    else
        printf("A kapott uzenet (gyerektol) kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext);
    return 0;
}

// receiving a message.
int gyerek(int uzenetsor)
{
    struct uzenet uz = {4, "asdasd"};
    struct uzenet2 uz2;
    int status;

    status = msgsnd(uzenetsor, &uz, strlen(uz.mtext) + 1, 0);
    if (status < 0)
        perror("msgsnd");

    status = msgrcv(uzenetsor, &uz2, sizeof(uz2.marr), 5, 0);
    if (status < 0)
        perror("msgsnd");
    else
        printf("A kapott uzenet (szulotol) kodja: %ld, szovege:\n", uz2.mtype);
    for (int i = 0; i < sizeof(uz2.marr) / sizeof(int); ++i)
    {
        printf("%d, ", uz2.marr[i]);
    }
    printf("\n");
    return 0;
}

int main(int argc, char *argv[])
{
    pid_t child;
    int uzenetsor, status;
    key_t kulcs;

    kulcs = ftok(argv[0], 1);
    printf("A kulcs: %d\n", kulcs);
    uzenetsor = msgget(kulcs, 0600 | IPC_CREAT);
    if (uzenetsor < 0)
    {
        perror("msgget");
        return 1;
    }

    child = fork();
    if (child > 0)
    {
        szulo(uzenetsor); // Parent sends a message.
        wait(NULL);
        // After terminating child process, the message queue is deleted.
        status = msgctl(uzenetsor, IPC_RMID, NULL);
        if (status < 0)
            perror("msgctl");
        return 0;
    }
    else if (child == 0)
    {
        return gyerek(uzenetsor);
        // The child process receives a message.
    }
    else
    {
        perror("fork");
        return 1;
    }

    return 0;
}