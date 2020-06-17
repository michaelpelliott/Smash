#define _GNU_SOURCE
/* ---------------------------------------------------
 * History.c for smash
 * 
 * Author: Michael Elliott
 * Date: 10/22/2019
 *
 * --------------------------------------------------*/
#include "history.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

static int sequenceNumber = 0;
static struct Cmd* head;
static struct Cmd* tail;
const int MAX = 10;
/* add_history adds an issued command in a struct Cmd node
 * into a single linked list of struct Cmd's */
void add_history(char* cmd, int exitStatus) {
    struct Cmd *p = malloc(sizeof(struct Cmd));
    if(sequenceNumber == 1) { // means the history is empty
        head = p;
    } else { // history is not empty
		tail->next = p;
	}
    p->exitStatus = exitStatus;
    p->seqNum = sequenceNumber++;
    p->cmd = strdup(cmd);
	p->next = NULL;
	tail = p;
	// this keeps the list at a maximum of 10 elements
    if(sequenceNumber > MAX+1) {
        struct Cmd* temp = head->next;
        free(head->cmd);
		free(head);
        head = temp;
    }
}

/* init_history will initialize the head, tail, and sequenceNumber
 * for the linked list history */
void init_history(void) {
	head = tail;
	sequenceNumber++;
}

/* clear_history will free all of the malloc'd memory from history.c */
void clear_history(void) {
	while (head != tail) {
		struct Cmd *temp = head->next;
		free(head->cmd);
		free(head);
		head = temp;
	}
	if(tail != NULL) {
		free(tail->cmd);
		free(tail);
	}
}
/* print_history prints from head of the list to tail */
void print_history(void) {
	struct Cmd *temp = head;
	while(temp->next != NULL) {
		printf("%d [%d] %s\n", temp->seqNum, temp->exitStatus, temp->cmd);
		temp = temp->next;
	}	
	printf("%d [%d] %s\n", temp->seqNum, temp->exitStatus, temp->cmd);
}
