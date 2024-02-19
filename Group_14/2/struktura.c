#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct driver
{
    unsigned int id;
    char name[50];
    char email[50];
    char phone[50];
    unsigned int exp;
};

// Id tracking
unsigned int id = 1;

void read_in_console_messages(const char *console_text, char *result)
{
    printf(console_text);
    scanf("\n%[^\t\n]s", result);
}

void file_creation()
{
    FILE *file = fopen("adatbazis.txt", "a"); // append to the end of the file
    fclose(file);
}

struct driver *file_read(FILE *file)
{
    struct driver *aktual = (struct driver *)malloc(sizeof(struct driver));
    // The C library function int fscanf(FILE *stream, const char *format, ...) reads formatted input from a stream.
    fscanf(file, "%u\t%[^\t]\t%s\t%s\t%u", &(aktual->id), aktual->name, aktual->email, aktual->phone, &(aktual->exp));
    return aktual;
}

void id_setup()
{
    FILE *infile;
    struct driver *aktual;

    id = 0;

    infile = fopen("adatbazis.txt", "r");
    if (infile != NULL)
    {
        aktual = file_read(infile);
        while (!feof(infile))
        {
            id = aktual->id;
			free(aktual);
            aktual = file_read(infile);
        }
		free(aktual);
    }
    fclose(infile);
    id++;
}

void driver_creation(const struct driver *createdriver)
{
    struct driver a;

    char beolvasott_ertek[50];
    unsigned int exp;
    if (createdriver != NULL)
    {
        a = *createdriver;
    }
    a.id = id;
    id++;
    read_in_console_messages("Name:\n", beolvasott_ertek);

    if (strcmp(beolvasott_ertek, "-\0") == 0) // összehasonlítani a két értéket, ha egyenlő 0
    {
        // elso parameter hova, masodik honnan
        strcpy(a.name, createdriver->name);
    }
    else
    {
        strcpy(a.name, beolvasott_ertek);
    }
    read_in_console_messages("Email:\n", beolvasott_ertek);
    if (strcmp(beolvasott_ertek, "-\0") == 0)
    {
        strcpy(a.email, createdriver->email);
    }
    else
    {
        strcpy(a.email, beolvasott_ertek);
    }
    read_in_console_messages("Telefonszam:\n", beolvasott_ertek);
    if (strcmp(beolvasott_ertek, "-\0") == 0)
    {
        strcpy(a.phone, createdriver->phone);
    }
    else
    {
        strcpy(a.phone, beolvasott_ertek);
    }

    printf("Tapasztalat:\n");
    scanf("%u", &exp);
    if (exp == 0)
    {
        a.exp = createdriver->exp;
    }
    else
    {
        a.exp = exp;
    }

    FILE *outfile;
    outfile = fopen("adatbazis.txt", "a");

    if (outfile == NULL)
    {
        fprintf(stderr, "\nHiba fajl megnyitaskor\n");
        exit(1);
    }
    fprintf(outfile, "%u\t%s\t%s\t%s\t%u\n", a.id, a.name, a.email, a.phone, a.exp);
    if (fprintf != 0)
    {
        printf("A felvetel sikeres volt !\n");
    }
    else
    {
        printf("Hiba az irasnal!\n");
    }

    fclose(outfile);
}

int main()
{
    int command;

    file_creation();
    id_setup();
    do
    {
        printf("-----MENU-----\n");
        printf("Driver_felvetel - 1\n");
        printf("Kilep - 2\n\n");
        switch (command)
        {
        case '1':
            driver_creation(NULL);
            break;
        case '2':
            printf("Kilepes\n");
            exit(0);
            break;
        default:
            break;
        }
    } while ((command = getchar()) != EOF);
    return 0;
}