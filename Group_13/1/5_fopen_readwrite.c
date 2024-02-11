#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>     //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno

int main()
{

    FILE *file_to_read = fopen("fourth_text.txt", "r");
    FILE *file_to_write = fopen("write_here.txt", "w");

    if (file_to_read == NULL || file_to_write == NULL)
    {
        printf("One file would not open!\n");
        return -1;
    }

    char c;
    while ((c = fgetc(file_to_read)) != EOF)
    {
        fputc(c, file_to_write);
    }
    fclose(file_to_read);
    fclose(file_to_write);

    return 0;
}