#include "sys/types.h"
#include "unistd.h"
#include "stdlib.h"
#include "signal.h"
#include "stdio.h"
#include "string.h"

pid_t mainszalertek = 0;

enum message
{
    counting
};

pid_t child(int pipe_id_rec, int pipe_id_send)
{
    pid_t szal = fork();
    if (szal == -1)
        exit(-1);
    if (szal > 0)
    {
        return szal;
    }

    enum message sign;
    int szamlalo = 1;
    while (szamlalo < 4)
    {
        read(pipe_id_rec, &sign, sizeof(enum message));
        printf("Child mondja: %i \n", szamlalo);
        szamlalo++;
    }

    exit(0);
}

int main(int argc, char **argv)
{
    mainszalertek = getpid();

    int io_pipes[2];
    int succ = pipe(io_pipes);
    if (succ == -1)
    {
        exit(-1);
    }

    int io_pipes1[2];
    int succ1 = pipe(io_pipes1);
    if (succ1 == -1)
    {
        exit(-1);
    }

    child(io_pipes1[0], io_pipes[1]);

    enum message puffer = counting;
    int mainszamlalo = 1;
    while (mainszamlalo < 4)
    {
        sleep(1);
        printf("Parent mondja: %i \n", mainszamlalo);
        write(io_pipes1[1], &puffer, sizeof(enum message));
        mainszamlalo++;
    }

    wait(NULL);
	close(io_pipes[0]);
    close(io_pipes[1]);
    close(io_pipes1[0]);
    close(io_pipes1[1]);
    return 0;
}
