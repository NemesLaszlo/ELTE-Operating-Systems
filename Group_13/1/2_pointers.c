#include <stdio.h>  // Be és kimenetekhez határoz meg változókat,makrókat és funckiókat.
#include <stdlib.h> // Általános függvény végrehajtásokhoz.

//argc = hány db karakter
//argv = argumentumok listája
int main(int argc, char **argv)
//char** means an array of character arrays = array of strings
{
    int n;
    n = 25;

    int *k = n; // pointer
    //int k = &n; // k az n memória címével

    printf("n = %i\n", n);
    //printf("k = %x\n", k);   // address
    printf("k = %i\n", k);

    return 0;
}