#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

pid_t mainszalertek = 0;
int fd;
char pipename[20];

pid_t child()
{
    pid_t szal = fork();
    if (szal == -1)
        exit(-1);
    if (szal > 0)
    {
        return szal;
    }

    fd = open(pipename, O_WRONLY);
    write(fd, "Test value\n", 12);
    close(fd);
    printf("Child - beirtam, vegeztem!\n");

    exit(0);
}

int main()
{
    mainszalertek = getpid();
    // sprintf - It formats and stores a series of characters and values in the specified buffer.
    // "/tmp/%d" - is a format string. In this format string, "/tmp/" is a constant string, and %d is a format specifier for an integer.
    // If "mainszalertek" has a value of, say, 42, after this line of code is executed, the pipename variable would contain the string "/tmp/42".
    sprintf(pipename, "/tmp/%d", mainszalertek);
    int fid = mkfifo(pipename, S_IRUSR | S_IWUSR); // creating named pipe file
    if (fid == -1)
    {
        printf("Error number: %i", errno);
        perror("Gaz van:");
        exit(EXIT_FAILURE);
    }

    char pufferchar[1024];

    child();
    fd = open(pipename, O_RDONLY);
    read(fd, &pufferchar, sizeof(pufferchar));
    printf("Szulo - Ezt olvastam a csobol: %s \n", pufferchar);
    close(fd);
    unlink(pipename);

    wait(NULL);
    return 0;
}
