/* -------------------------------------------
**
**	Smash.c 
**
**	Author: Michael Elliott
**	Class: CS253, Fall 2019
**
**
** ------------------------------------------*/

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "smash.h"
#include "history.h"

#define MAXLINE 4096 
int exitStatus = 10; // Assume failure
char bfr[MAXLINE];
int historyFlag = 0; // so that history is initiated only once

int main(int argc, char **argv) {
	fputs("$ ", stderr);
    while (fgets(bfr,MAXLINE,stdin) != NULL && exitStatus != 42) {
	    bfr[strlen(bfr) - 1] = '\0'; //replace newline with null
        if(historyFlag == 0) {
            historyFlag++;
			init_history();
        }
        exitStatus = executeCommand(bfr);
	    if(exitStatus == 42) {
			exitStatus = 0;
            clear_history();
            exit(exitStatus);
        }
        else { 
            fputs("$ ", stderr);
        }
    }
	return exitStatus;
}
