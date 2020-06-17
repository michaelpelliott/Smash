/* -------------------------------------------
 *  History Header File
 *
 *  Contains function prototypes for the 
 *  history command to be used with 
 *  the smash executable
 *
 *  Author: Michael Elliott
 *  Date: 10/23/2019
 *
 * -------------------------------------------*/

#ifndef HISTORY_H
#define HISTORY_H

#define MAXHISTORY 10 // Smash will save the history of the last 10 commands
// Layout of a single entry in the history array
struct Cmd {
    struct Cmd* next;
    int seqNum;     // Sequence number the command
    char* cmd;      // A saved copy of the users command string
    int exitStatus; // The exit status from this command
};


// Function prototypes
void init_history(void); //Builds data structures for recording cmd history
void add_history(char* cmd, int exitStatus); //Adds an entry to the history
void clear_history(void); //Frees all malloc'd memory in the history
void print_history(void); //Prints the history to stdout

#endif
