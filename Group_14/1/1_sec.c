#include <stdio.h>  // Be és kimenetekhez határoz meg változókat,makrókat és funckiókat.
#include <stdlib.h> // Általános függvény végrehajtásokhoz.
//call it with some parameters from the command line

//argc = hány db karakter
//argv = argumentumok listája
int main(int argc, char **argv)
//char** means an array of character arrays = array of strings
{
    int i;
    printf("Number of command line arguments are: %i\n", argc);
    for (i = 0; i < argc; i++)
    {
        printf("%i. argument is %s\n", i, argv[i]);
    }
    return 0;
}