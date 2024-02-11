#include <stdio.h>  // Be és kimenetekhez határoz meg változókat,makrókat és funckiókat.
#include <stdlib.h> // Általános függvény végrehajtásokhoz.

//argc = hány db karakter
//argv = argumentumok listája
int main(int argc, char **argv)
//char** means an array of character arrays = array of strings
{
    int num1 = 6;
    int num2 = 5;
    char char1 = 'A';

    printf("Values of this two var are: %i and %i\n", num1, num2);
    printf("Char is: %c\n", char1);
    printf("Char is: %i\n", char1); //ascii

    int result = 0;
    result = num1 + num2;

    printf("Result: %i\n", result);

    return 0;
}

/*
%d takes integer value as signed decimal integer i.e. it takes negative values along with positive values 
but values should be in decimal otherwise it will print garbage value.

%i takes integer value as integer value with decimal, hexadecimal or octal type.
*/

/*
 signed by default, meaning it can represent both positive and negative values.
 unsigned is an integer that can never be negative.
*/