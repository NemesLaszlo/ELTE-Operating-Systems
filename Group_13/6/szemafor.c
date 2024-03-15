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

int szemafor_letrehozas(const char *pathname, int szemafor_ertek)
{
    int semid;
    key_t kulcs;

    kulcs = ftok(pathname, 1);
    if ((semid = semget(kulcs, 1, IPC_CREAT | S_IRUSR | S_IWUSR)) < 0) // semget - creates a semaphore set -> second parameter is 1, we only need one semaphore
        perror("semget");
    // semget 2. parameter is the number of semaphores
    if (semctl(semid, 0, SETVAL, szemafor_ertek) < 0)
        perror("semctl");

    return semid;
}

void szemafor_muvelet(int semid, int op)
{
    struct sembuf muvelet;

    muvelet.sem_num = 0; // This member specifies the index of the semaphore within the semaphore set, now we only have one, so it is a "hardcoded" value
    muvelet.sem_op = op; // op=1 up, op=-1 down
    muvelet.sem_flg = 0; // SEM_UNDO, IPC_NOWAIT
	// For example:
	// SEM_UNDO: Automatically performs an undo operation on the semaphore when the process exits or terminated.
	// IPC_NOWAIT: Performs a non-blocking operation. If the semaphore operation would block, the operation fails immediately.
	
	// Positive value: Increments the semaphore value by sem_op.
	// Negative value: Decrements the semaphore value by the absolute value of sem_op.
	// Zero: Checks if the semaphore value is zero. If it is zero, the operation blocksm otherwise it returns inmediately.

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
    char *s;

    kulcs = ftok(argv[0], 1);
    sh_mem_id = shmget(kulcs, MEMSIZE, IPC_CREAT | S_IRUSR | S_IWUSR); // shared mem
    s = shmat(sh_mem_id, NULL, 0);
    semid = szemafor_letrehozas(argv[0], 0); // sem state is down!!!
	
    child = fork();
    if (child > 0)
    {
        char buffer[] = "I like something, test sentence!\n";
        printf("Szulo indul, kozos memoriaba irja: %s\n", buffer);
        strcpy(s, buffer);
        printf("Szulo, szemafor up!\n");
        szemafor_muvelet(semid, 1); // Up
        shmdt(s);                   // release shared memory
        wait(NULL);
        szemafor_torles(semid);
        shmctl(sh_mem_id, IPC_RMID, NULL);
    }
    else if (child == 0)
    {

        // critical section
        printf("Gyerek: Indula szemafor down!\n");
        szemafor_muvelet(semid, -1); // down, wait if necessary
        printf("Gyerek, down rendben, eredmeny: %s", s);
        szemafor_muvelet(semid, 1); // up
        // end of critical section
        shmdt(s);
    }

    return 0;
}
