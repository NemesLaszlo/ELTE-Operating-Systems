### Forks

Fork system call is used for creating a new process, which is called child process, which runs concurrently with the process that makes the fork() call (parent process). After a new child process is created, both processes will execute the next instruction following the fork() system call. A child process uses the same pc (program counter), same CPU registers, same open files which use in the parent process.
It takes no parameters and returns an integer value. Below are different values returned by fork().

- *Negative Value:* creation of a child process was unsuccessful.

- *Zero:* Returned to the newly created child process.

- *Positive value:* Returned to parent or caller. The value contains process ID of newly created child process.

```

#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h> 
int main() 
{ 

	// make two process which run same 
	// program after this instruction 
	fork(); 

	printf("Hello world!\n"); 
	return 0; 
}

```

```
Output:
Hello world!
Hello world!
```

#### Other example (1):

```

#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h> 

void forkexample() 
{ 
	// child process because return value zero 
	if (fork() == 0) 
		printf("Hello from Child!\n"); 

	// parent process because return value non-zero. 
	else
		printf("Hello from Parent!\n"); 
} 
int main() 
{ 
	forkexample(); 
	return 0; 
}

```

```
Output:

1.
Hello from Child!
Hello from Parent!

(or)

2.
Hello from Parent!
Hello from Child!
```

In the above code, a child process is created. fork() returns 0 in the child process and positive integer in the parent process.

Here, two outputs are possible because the parent process and child process are running concurrently. So we don’t know whether the OS will first give control to the parent process or the child process.

#### Other example (2):

```

#include <stdio.h> 				
#include <sys/types.h> 

int main() 
{ 
	fork(); 
	fork(); 
	fork(); 
	printf("hello\n"); 
	return 0; 
}

```

```
Output: (8db hello)
```

```
fork ();   // Line 1
fork ();   // Line 2
fork ();   // Line 3

       L1       // There will be 1 child process 
    /     \     // created by line 1.
  L2      L2    // There will be 2 child processes
 /  \    /  \   //  created by line 2
L3  L3  L3  L3  // There will be 4 child processes 
                // created by line 3
```


### Signals

Signals are software interrupts sent to a program to indicate that an important event has occurred. The events can vary from user requests to illegal memory access errors. Some signals, such as the interrupt signal, indicate that a user has asked the program to do something that is not in the usual flow of control.

| Signal Name | Signal Number | Description                                                |
|-------------|---------------|------------------------------------------------------------|
| SIGHUP      | 1             | Hang up detected on controlling terminal or death of controlling process |
| SIGINT      | 2             | Issued if the user sends an interrupt signal (Ctrl + C)    |
| SIGQUIT     | 3             | Issued if the user sends a quit signal (Ctrl + D)          |
| SIGFPE      | 8             | Issued if an illegal mathematical operation is attempted   |
| SIGKILL     | 9             | If a process gets this signal it must quit immediately and will not perform any clean-up operations |
| SIGALRM     | 14            | Alarm clock signal (used for timers)                       |
| SIGTERM     | 15            | Software termination signal (sent by kill by default)      |

There is an easy way to list down all the signals supported by your system. Just issue the ```kill -l``` command and it would display all the supported signals.

Every signal has a default action associated with it. The default action for a signal is the action that a script or program performs when it receives a signal.

There are several methods of delivering signals to a program or script. One of the most common is for a user to type CONTROL-C or the INTERRUPT key while a script is executing.

When you press the Ctrl+C key, a SIGINT is sent to the script and as per defined default action script terminates.


The other common method for delivering signals is to use the kill command, the syntax of which is as follows:

```

kill -signal pid

```

#### Signal - Traps:

When you press the Ctrl+C or Break key at your terminal during execution of a shell program, normally that program is immediately terminated, and your command prompt returns. This may not always be desirable. For instance, you may end up leaving a bunch of temporary files that won't get cleaned up.

As an example of the trap command, the following shows how you can remove some files and then exit if someone tries to abort the program from the terminal

```

trap "rm -f $WORKDIR/work1$$ $WORKDIR/dataout$$; exit" 2

```

- Other:

```

trap ’echo Testing...’ 2

Back to default definition: trap 2

```


