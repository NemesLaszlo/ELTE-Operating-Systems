#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
Az osztott vagy közös memória segítségével megoldható, hogy két vagy több folyamat ugyanazt a memóriarészt használja. Az osztott memóriazónák általi kommunikáció elvei:

- Egy folyamat létrehoz egy közös memóriazónát. A folyamat azonosítója bekerül a memóriazónához rendelt struktúrába.

- A létrehozó folyamat hozzárendel az osztott memóriához egy numerikus kulcsot, amelyet minden ezt a memóriarészt használni kívánó folyamatnak ismernie kell. Ezt a memóriazónát az shmid változó azonosítja.

- A létrehozó folyamat leszögezi a többi folyamat hozzáférési jogait az illető zónához. Azért, hogy egy folyamat (beleértve a létrehozó folyamatot is) írni és olvasni tudjon a közös memóriarészből, hozzá kell rendelnie egy virtuális címterületet.

Ez a kommunikáció a leggyorsabb, hiszen az adatokat nem kell mozgatni a kliens és a szerver között.
*/

int main(int argc, char *argv[])
{

   int pid;
   key_t kulcs;
   int oszt_mem_id;
   char *s;
   // a parancs nevevel es az 1 verzio szammal kulcs generalas
   kulcs = ftok(argv[0], 1);
   // osztott memoria letrehozasa, irasra olvasasra, 500 bajt mrettel
   oszt_mem_id = shmget(kulcs, 500, IPC_CREAT | S_IRUSR | S_IWUSR);
   // csatlakozunk az osztott memoriahoz,
   // a 2. parameter akkor kell, ha sajat cimhez akarjuk illeszteni
   // a 3. paraméter lehet SHM_RDONLY, ekkor csak read van
   s = shmat(oszt_mem_id, NULL, 0);
   //
   pid = fork();
   if (pid > 0)
   {

      char buffer[] = "Valami iras! \n";
      // beirunk a memoriaba
      strcpy(s, buffer);
      // elengedjuk az osztott memoriat
      shmdt(s);
      //	s[0]='B';  //ez segmentation fault hibat ad
      wait(NULL); //megvárjuk, hogy a gyerek kiolvassa
      // IPC_RMID- torolni akarjuk a memoriat, ekkor nem kell 3. parameter
      // IPC_STAT- osztott memoria adatlekerdezes a 3. parameterbe,
      shmctl(oszt_mem_id, IPC_RMID, NULL); // shared memory control
   }
   else if (pid == 0)
   {
      sleep(1);
      printf("A gyerek ezt olvasta az osztott memoriabol: %s", s);
      // gyerek is elengedi
      shmdt(s);
   }

   return 0;
}
