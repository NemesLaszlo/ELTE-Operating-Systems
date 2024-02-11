#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>     //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno

int main()
{

    printf("Write something!\n");
    char s[100];
    int g = open("seventh_test_file.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (g < 0)
    {
        perror("Error at opening the file\n");
        exit(1);
    }
    do
    {

        printf("Enter: ");
        fgets(s, sizeof(s), stdin); //beolvasás módszer fix mérettel: input\n -t olvas
        printf("Input: %s", s, "\n");

        int length = strlen(s);

        if (write(g, &s, length) != length)
        {
            perror("There is a mistake in writing\n");
            exit(1);
        }

    } while (strcmp(s, "over\n") != 0);

    close(g);
    return 0;
}