//Read and print out the file given in the parameter
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>     //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>
#include <unistd.h> // access

// fopen egy library call, open egy syscall, fopen egy magasabb szintű absztrakció, és az fopen használl buffered i/o amiért
// az gyorsabb mint az open, fopen is az opent hívja meg egyébként.
// pipeok esetén pl. a buffering nem kifizetődő, hiszen rögtön küldenénk, így hejzet függő melyiket használjuk.

void use_open_bin(char *fname)
{
  printf("\n**********\nUsing open - binary \n****************\n");
  int f = open(fname, O_RDONLY); // open esetén mode string mellett mode flageket is meg lehet adni mint itt.
  if (f < 0)
  {
    perror("Error at opening the file\n");
    exit(1);
  }

  char c; //instead of char, you can use any other type, struct as well
  while (read(f, &c, sizeof(c)))
  {                  //use write for writing
    printf("%c", c); //we prints out the content of the file on the screen
  }
  printf("\n");
  lseek(f, 0, SEEK_CUR);
  /*
  SEEK_SET	Offset is to be measured in absolute terms.
  SEEK_CUR	Offset is to be measured relative to the current location of the pointer.
  SEEK_END	Offset is to be measured relative to the end of the file.
  */
  read(f, &c, sizeof(c));
  printf("Current character - after positioning %c\n", c);
  close(f);
}

void use_fopen_text(char *fname)
{
  printf("\n******************\nUsing fopen -  reads in lines\n*****************\n");
  FILE *f;
  f = fopen(fname, "r");
  if (f == NULL)
  {
    perror("File opening error\n");
    exit(1);
  }
  char line[160];
  while (!feof(f)) // fájl vége vizsgálat
  {
    fgets(line, sizeof(line), f); // sor beolvasása megáll sor vége karakternél vagy fájl végénél
    printf("%s", line);
  }
  printf("\n");
  fclose(f);
}

void use_fopen_bin(char *fname)
{
  printf("\n******************\nUsing fopen -  binary \n*****************\n");
  FILE *f;
  f = fopen(fname, "rb");
  if (f == NULL)
  {
    perror("File opening error\n");
    exit(1);
  }
  char c; //instead of char, you can use any other type, struct as well
  while (!feof(f))
  {
    fread(&c, sizeof(c), sizeof(c), f); //use fwrite for writing
    /*
    első sizeof - This is the size in bytes of each element to be read.
    második sizeof - This is the number of elements, each one with a size of első sizeof bytes.
    */
    printf("%c", c);
  }
  printf("\n");
  fseek(f, 0, SEEK_SET); //position to the first character, SEEK_SET,SEEK_CUR,SEEK_END
  fread(&c, sizeof(c), sizeof(c), f);
  printf("First character - after positioning %c\n", c);

  fclose(f);
}

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    perror("Give a filename as a command line argument\n");
    exit(1);
  }
  if (!access(argv[1], F_OK) == 0)
  {
    perror("The file is not exist!\n");
    exit(1);
  }
  use_fopen_text(argv[1]);
  use_open_bin(argv[1]);
  use_fopen_bin(argv[1]);
  return 0;
}