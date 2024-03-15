#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MEMSIZE 1024

struct sharedData
{
    char text[MEMSIZE];
    int textSize;
};

int szemafor_letrehozas(const char *pathname, int szemafor_ertek)
{
    int semid;
    key_t kulcs;

    kulcs = ftok(pathname, 1);
    if ((semid = semget(kulcs, 1, IPC_CREAT | S_IRUSR | S_IWUSR)) < 0)
        perror("semget");
    // semget 2. parameter is the number of semaphores
    if (semctl(semid, 0, SETVAL, szemafor_ertek) < 0) //0= first semaphores
        perror("semctl");

    return semid;
}

void szemafor_muvelet(int semid, int op)
{
    struct sembuf muvelet;

    muvelet.sem_num = 0;
    muvelet.sem_op = op; // op=1 up, op=-1 down
    muvelet.sem_flg = 0;

    if (semop(semid, &muvelet, 1) < 0) // 1 number of sem. operations
        perror("semop");
}

void szemafor_torles(int semid)
{
    semctl(semid, 0, IPC_RMID);
}

int main(int argc, char *argv[])
{

    pid_t child;
    key_t kulcs;
    int sh_mem_id, semid;
    struct sharedData *s;

    kulcs = ftok(argv[0], 1);
    sh_mem_id = shmget(kulcs, sizeof(s), IPC_CREAT | S_IRUSR | S_IWUSR);
    s = shmat(sh_mem_id, NULL, 0);

    semid = szemafor_letrehozas(argv[0], 1);

    child = fork();
    if (child > 0)
    {
        int running = 1;

        while (running)
        {
            printf("Parent: reading to buffer...\n");

            char buffer[MEMSIZE];
            fgets(buffer, MEMSIZE, stdin);
            buffer[strlen(buffer) - 1] = 0;
            if (strcmp(buffer, "exit") == 0)
            {
                running = 0;
            }
            printf("Parent: read to buffer: %s\n", buffer);
            szemafor_muvelet(semid, -1); // Down
            strcpy(s->text, buffer);
            printf("Parent: previous length: %i\n", s->textSize);
            szemafor_muvelet(semid, 1); // Up
        }

        shmdt(s); // release shared memory
        wait(NULL);
        szemafor_torles(semid);
        shmctl(sh_mem_id, IPC_RMID, NULL);
    }
    else if (child == 0)
    {
        int running = 1;

        while (running)
        {
            sleep(2);
            printf("Child: Waiting...\n");

            szemafor_muvelet(semid, -1); // Down
            printf("Child: Reading: %s\n", s->text);
            if (strcmp(s->text, "exit") == 0)
            {
                running = 0;
            }
            s->textSize = strlen(s->text);
            szemafor_muvelet(semid, 1); // Up
        }
        shmdt(s);
    }

    return 0;
}
