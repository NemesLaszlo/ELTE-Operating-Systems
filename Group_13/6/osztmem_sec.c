#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *s;
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

    printf("A gyerek ezt olvasta az osztott memoriabol: %s", s);
    // gyerek is elengedi
    shmdt(s);

    exit(0);
}

int main(int argc, char *argv[])
{

    int pid;
    key_t kulcs;
    int oszt_mem_id;
    // a parancs nevevel es az 1 verzio szammal kulcs generalas
    kulcs = ftok(argv[0], 1);
    // osztott memoria letrehozasa, irasra olvasasra, 500 bajt mrettel
    oszt_mem_id = shmget(kulcs, 500, IPC_CREAT | S_IRUSR | S_IWUSR);
    // csatlakozunk az osztott memoriahoz,
    // a 2. parameter akkor kell, ha sajat cimhez akarjuk illeszteni
    // a 3. paraméter lehet SHM_RDONLY, ekkor csak read van
    s = shmat(oszt_mem_id, NULL, 0);

    mainszalertek = getpid();

    child1();

    char buffer[] = "Valami iras! \n";
    // beirunk a memoriaba
    strcpy(s, buffer);
    // elengedjuk az osztott memoriat
    shmdt(s);

    wait(NULL);
    // IPC_RMID- torolni akarjuk a memoriat, ekkor nem kell 3. parameter
    shmctl(oszt_mem_id, IPC_RMID, NULL); // shared memory control

    return 0;
}