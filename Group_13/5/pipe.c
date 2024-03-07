#include "sys/types.h"
#include "unistd.h"
#include "stdlib.h"
#include "signal.h"
#include "stdio.h"
#include "string.h"
#include "time.h"

pid_t mainszalertek = 0;

pid_t child(int pipe_id_rec, int pipe_id_send)
{
    pid_t szal = fork();
    if (szal == -1)
        exit(-1);
    if (szal > 0)
    {
        return szal;
    }

    int arrived;
    char str[] = "Child kuldi a szulonek: Megkaptam a szamot!";
    read(pipe_id_rec, &arrived, sizeof(int));
    printf("Child olvasta uzenet: %i", arrived);
    printf("\n");
    write(pipe_id_send, str, sizeof(str));

    exit(0);
}

int main()
{

    int io_pipes[2];
    int succ = pipe(io_pipes);
    if (succ == -1)
        exit(-1);

    int io_pipes1[2];
    int succ1 = pipe(io_pipes1);
    if (succ1 == -1)
        exit(-1);

    mainszalertek = getpid();
    char pufferchar[50];

    child(io_pipes1[0], io_pipes[1]); // 0 ban fogad 1 essel kuld

    srand(time(NULL));
    int r = rand() % 100; // number between 0 and 99
    printf("Random number %i", r);
    printf("\n");

    write(io_pipes1[1], &r, sizeof(r));
    printf("Szulo beirta az adatokat a csobe!\n");

    read(io_pipes[0], pufferchar, 50);
    printf("Child mondja, (Szulo kapta): %s \n", pufferchar);

    wait(NULL);
    close(io_pipes1[0]);
    close(io_pipes1[1]);
    close(io_pipes[0]);
    close(io_pipes[1]);
    return 0;
}
