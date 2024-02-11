# ELTE-Operating-Systems
This repository contains the tasks / files and details of the ELTE Operating systems course.

### ELTE server
- Server: opsys.inf.elte.hu
- Login: The `neptun code` in lowercase and then the password (infes azonosító)

### Content and thematic:
- Basics of C, file management, binary, line-by-line file reading, pointers, "strings" etc.
- Forks and processes with C
- Handling of signals
- Use of pipes between processes (pipes, named pipes)
- Use of message queues between processes
- Shared memory, Semaphores
- Tasks summarizing all previously mentioned topics, "where a given situation must be simulated”.

### Compilement: 

```gcc``` and the name of the .c file. -> default output ```a.out```
We can run it as ./a.out

Different switches:

• -o switch to specify an output file option instead of the default a.out

e.g. ```gcc arg.c -o first``` and then we can run it as ```./first```

• -Werror switch, for displaying different errors

e.g. ```gcc -Werror arg.c -o first```

• Wall switch, which checks both errors and warnings

e.g. ```gcc -Wall arc.c -o first```