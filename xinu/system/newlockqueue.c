/* newlockqueue.c  */

#include <xinu.h>
#include <stdlib.h>

/**
 * Allocate and initialize a queue
 * @return pointer to queue structure
 */
struct lqueue *newlqueue(void)
{
	// allocate memory for a new Queue structure (required to be on heap)
	struct lqueue *newlqueue = (struct lqueue*) malloc(sizeof(struct lqueue));

	// initialize the structure
	newlqueue->head = NULL;
	newlqueue->tail = NULL;
	newlqueue->lqsize = 0;

	// return pointer to the structure
	return newlqueue;
}
