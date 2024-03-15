#include "sys/types.h"
#include "unistd.h"
#include "stdlib.h"
#include "signal.h"
#include "stdio.h"
#include "string.h"
#include "time.h"

pid_t mainszalertek = 0;
int child_communication_in[2];
int child_communication_out[2];

int rand_num(int max)
{
    return rand() % max;
}

pid_t child_1(int pipe_id_rec, int pipe_id_send)
{
    pid_t szal = fork();
    if (szal == -1)
        exit(-1);
    if (szal > 0)
    {
        return szal;
    }

    int arrived;
    read(pipe_id_rec, &arrived, sizeof(int)); // fogadja a számot
    printf("Child_1 olvasta uzenet: %i \n", arrived);

    write(child_communication_out[1], &arrived, sizeof(int));
    int i = 0;
    srand(time(NULL));

    while (i < arrived)
    {
        int random_num = rand_num(1000);
        write(child_communication_out[1], &random_num, sizeof(int));
        i++;
    }

    exit(0);
}

pid_t child_2(int pipe_id_rec, int pipe_id_send)
{
    pid_t szal = fork();
    if (szal == -1)
        exit(-1);
    if (szal > 0)
    {
        return szal;
    }

    int arrived;
    read(pipe_id_rec, &arrived, sizeof(int)); // fogadja a számot
    printf("Child_2 olvasta uzenet: %i \n", arrived);

    int i = 0;
    int buffer = 0;
    while (i < arrived)
    {
        read(pipe_id_rec, &buffer, sizeof(int));
        printf("Child_2 olvasta random szam: %i \n", buffer);
        i++;
    }

    exit(0);
}

int main(int argc, char **argv)
{
    mainszalertek = getpid();

    int console_num = atoi(argv[1]);

    int io_pipes_in[2];
    int succ = pipe(io_pipes_in);
    if (succ == -1)
        exit(-1);

    int io_pipes_out[2];
    int succ1 = pipe(io_pipes_out);
    if (succ1 == -1)
        exit(-1);

    int succ2 = pipe(child_communication_in);
    if (succ2 == -1)
        exit(-1);

    int succ3 = pipe(child_communication_out);
    if (succ3 == -1)
        exit(-1);

    child_1(io_pipes_out[0], io_pipes_in[1]);                       // 0 ban fogad 1 ben küld
    child_2(child_communication_out[0], child_communication_in[1]); // nevek zavaróak lehetnek az in, out főleg azért kapták a neveket, hogy amikor használjuk az ellenkező vég kell vagyis out[1] küld (child-ban eleve beszédes a neve)

    write(io_pipes_out[1], &console_num, sizeof(int)); // child_1 nek elküldi a számot

    wait(NULL);
	close(io_pipes_in[0]);
    close(io_pipes_in[1]);
    close(io_pipes_out[0]);
    close(io_pipes_out[1]);
	close(child_communication_in[0]);
    close(child_communication_in[1]);
    close(child_communication_out[0]);
    close(child_communication_out[1]);
    return 0;
}