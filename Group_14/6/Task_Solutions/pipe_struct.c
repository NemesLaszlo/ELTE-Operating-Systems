#include "sys/types.h"
#include "unistd.h"
#include "stdlib.h"
#include "signal.h"
#include "stdio.h"
#include "string.h"

struct worker
{
	int id;
	char name[500];
	char address[500];
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
	char puffer[100];
	int daily_num;

	read(pipe_id_rec, &puffer, 100);
	read(pipe_id_rec, &daily_num, sizeof(int));
	printf("Child: A %s munka ennyi emberrel: %i kerül elvégzésre.\n", puffer, daily_num);

	struct worker *data = malloc(daily_num * sizeof(struct worker));
	read(pipe_id_rec, data, daily_num * sizeof(struct worker));
	int j = 0;
	while (j < daily_num)
	{
		printf("Received: Id: %i and Name: %s and Address: %s\r\n", data[j].id, data[j].name, data[j].address);
		j++;
	}
	free(data);
	exit(0);
}

int main(int argc, char **argv)
{

	int io_pipes_in[2];
	int succ = pipe(io_pipes_in);
	if (succ == -1)
	{
		exit(-1);
	}

	int io_pipes_out[2];
	int succ1 = pipe(io_pipes_out);
	if (succ1 == -1)
	{
		exit(-1);
	}

	child(io_pipes_out[0], io_pipes_in[1]); // 0 ban fogad 1 el küld

	// pár adat, ami az adott naphoz tartozik
	struct worker monday_datas[3] = {
		{1, "Bela", "Budapest"},
		{2, "Janos", "Miskolc"},
		{3, "Mark", "Szeged"},
	};

	int daily_num_ready = 3;
	// foglalunk egy adott memóriaterületet a megadott munkásszámnak megfelelően, pl. 3 munkás 3 szor kell a worker struktúrányi terület.
	struct worker *daily_datas = malloc(daily_num_ready * sizeof(struct worker));

	// az üres memóriaterületre írjuk az adatokat
	int i = 0;
	while (i < daily_num_ready)
	{
		daily_datas[i] = monday_datas[i];
		i++;
	}

	char actual_work[100] = "Fertotlenites";
	write(io_pipes_out[1], &actual_work, 100);									  // elkuldjuk mi a munka
	write(io_pipes_out[1], &daily_num_ready, sizeof(int));						  // a szamot, hogy hany munkas lesz aznap
	write(io_pipes_out[1], daily_datas, daily_num_ready * sizeof(struct worker)); // és a munkás struktúrákat

	wait(NULL);
	free(daily_datas);
	close(io_pipes_in[0]);
    close(io_pipes_in[1]);
    close(io_pipes_out[0]);
    close(io_pipes_out[1]);
	return 0;
}
