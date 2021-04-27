/* lockqueue.h */

/* Queue structure declarations, constants, and inline functions	*/

/* Default # of queue entries: 1 per process plus 2 for ready list plus	*/
/*			2 for sleep list plus 2 per semaphore		*/

#ifndef NQENT
#define NQENT	(NPROC + 4 + NSEM + NSEM)
#endif

#define	EMPTY	(-1)		/* null value for qnext or qprev index	*/


// define queue's data members
struct lqueue
{
	//  pointer to head qentry
	struct lqentry* head;

	//  pointer to tail qentry
	struct lqentry* tail;

	//  size of queue
	unsigned int lqsize;

};


// define a queue entry's data members
struct lqentry
{
	//  process ID
	pid32 pid;
	int32 key;

	//  other members
	struct lqentry* prev;
	struct lqentry* next;
};


/* Queue function prototypes (don't touch!) */
void    printlqueue(struct lqueue*);
int32 	lsize(struct lqueue*);

bool8	lisempty(struct lqueue*);
bool8	lnonempty(struct lqueue*);
bool8	lisfull(struct lqueue*);

pid32	lgetfirst(struct lqueue*);
pid32	lgetlast(struct lqueue*);
pid32	lremove(pid32, struct lqueue*);
pid32	ldequeue(struct lqueue*);
pid32 	lenqueue(pid32, struct lqueue*, int32);

struct lqueue	*newlqueue();
struct lqentry	*lgetbypid(pid32, struct lqueue*);
