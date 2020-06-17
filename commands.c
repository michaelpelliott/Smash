#define _GNU_SOURCE
/* ---------------------------------------
 * Commands.c for smash
 *
 * Author: Michael Elliott
 * Class: CS253, Fall 2019
 *
 *  --------------------------------------*/

#include "smash.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h" // for strlen for testing
#include "unistd.h" // for chdir, execvp
#include "history.h"
#include "sys/wait.h" // for wait

#define MAX 4096 

char* dirname;
char* args[MAX]; // may need to make it MAX+1 and make args[MAX+1] NULL
int exitState = 1;
char* argString;
char* argStringEC;
char* argsEC[MAX];
/* executeCommand tokenizes the string from smash.c and builds the 
 * args array from it. It then executes the command issued, if it can. */
int executeCommand(char *str) {
	argString = strdup(str);
	const char sep[2] = " ";
    char *token;
    int i = 0;
    token = strtok(str, sep);
    while (token != NULL) {		// populates the args array
        args[i] = token;
        i++;
        token = strtok(NULL, sep);
    }
    args[i] = NULL;
	if(args[0] == '\0') {
		exitState = 0;
	}
	else {		// process the args array, execute commands
		if (strcmp(args[0],"exit") == 0) {
        	exitState = 42;		// Dummy exit status is a flag for smash.c to exit(0)
    	}
    	else if(strcmp(args[0],"cd") == 0 && args[1] != NULL) {
        	exitState = chdir(args[1]); // assume success, exitState = 0 on success;
        	if(exitState < 0) { // chdir failed
            	perror(args[1]);
            	exitState = 13; // fail but not bad enough error to quit the program
        	} 
			else { // chdir was successful
            	printf("%s\n", args[1]);
        	}
        	add_history(argString, exitState);
    	}
    	else if(strcmp(args[0],"history") == 0) {
        	exitState = 0;
			add_history(argString, exitState);
			print_history();
    	}
    	else { // external command issued
           exitState = executeExternalCommand(argString);
		}
	}
	free(argString);
return exitState;
}
/* executeExternalCommand will fork, wait, and execvp 
 * for the executeCommand function.
 * Parameters: the string of the args array to be tokenized inside the function
 */
int executeExternalCommand(char* argString) {
	int exitState = -1; // assume failure
	int kidFlag = -1; // for catching a floating child
	argStringEC = strdup(argString); 
	const char sepEC[2] = " ";
    char *tokenEC;
    int i = 0;
    tokenEC = strtok(argString, sepEC);

	if(strlen(tokenEC) > MAX-3) {
		printf("File name too long\n"); // add smash: before each of these
		exitState = -2;			// -2: special exitState of input too large
	}
	else {		
    	while (tokenEC != NULL) {		// populates the args array
        	argsEC[i] = tokenEC;
        	i++;
        	tokenEC = strtok(NULL, sepEC);
    	}
    	argsEC[i] = NULL;
    	setvbuf(stdout,NULL,_IONBF,0);
    	setvbuf(stderr,NULL,_IONBF,0);
    	int pid = fork();
    	if(pid > 0) { // parent
        	int exitResult;
        	int kidPid = wait(&exitResult);
        	if (kidPid > 0) { //successful wait
            	exitState = WEXITSTATUS(exitResult);
			}
        	else { // failed wait
            	perror(argsEC[0]); // perror("smash");
        	}
    	}
    	else if(pid == 0) { // child
			kidFlag = 1; 
			int exe = execvp(argsEC[0], argsEC);
        	if(exe) {	// execvp failed
				exitState = exe;
				perror(argsEC[0]); // perror("smash");
    		}
		}
    	else { // fork error
        	perror(argsEC[0]);
    	}
		if(kidFlag > 0) { // child process must exit and not remain alive
			add_history(argStringEC, exitState);
			exitState = 42; // dummy exit status for main to exit and terminate the child
		}
		else {
			add_history(argStringEC, exitState);
		}
	}
	free(argStringEC);
    return exitState;
}
