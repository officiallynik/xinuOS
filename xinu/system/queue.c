/* queue.c - enqueue, dequeue */

#include <xinu.h>

struct qentry	queuetab[NQENT];	/* Table of process queues	*/

/*------------------------------------------------------------------------
 *  enqueue  -  Insert a process at the tail of a queue
 *------------------------------------------------------------------------
 */
pid32	enqueue(
	  pid32		pid,		/* ID of process to insert	*/
	  qid16		q		/* ID of queue to use		*/
	)
{
	qid16	tail, prev;		/* Tail & previous node indexes	*/

	if (isbadqid(q) || isbadpid(pid)) {
		return SYSERR;
	}

	tail = queuetail(q);
	prev = queuetab[tail].qprev;

	queuetab[pid].qnext  = tail;	/* Insert just before tail node	*/
	queuetab[pid].qprev  = prev;
	queuetab[prev].qnext = pid;
	queuetab[tail].qprev = pid;
	return pid;
}

/*------------------------------------------------------------------------
 *  dequeue  -  Remove and return the first process on a list
 *------------------------------------------------------------------------
 */
pid32	dequeue(
	  qid16		q		/* ID of queue to use		*/
	)
{
	pid32	pid;			/* ID of process removed	*/

	if (isbadqid(q)) {
		return SYSERR;
	} else if (isempty(q)) {
		return EMPTY;
	}

	pid = getfirst(q);
	queuetab[pid].qprev = EMPTY;
	queuetab[pid].qnext = EMPTY;
	return pid;
}

/**
 * Finds a queue entry given pid
 * @param pid	a process ID
 * @param q	a pointer to a queue
 * @return pointer to the entry if found, NULL otherwise
 */
struct qentry *getbypid(pid32 pid, qid16 q)
{
	//TODO - return NULL if queue is empty or if an illegal pid is given
	if(isempty(q) == TRUE || isbadpid(pid) == TRUE) {
		return NULL;
	}

	struct qentry *iter = queuehead(q);
	
	//if first element
	while(iter != NULL) {
		
		if(iter->pid == pid) {
			return iter;
		}

		//TODO - find the qentry with the given pid
		else {
			iter = iter.qnext;
		}
	}

	return NULL;
}

/**
 * Remove a process from an arbitrary point in a queue
 * @param pid	ID of process to remove
 * @param q	Pointer to the queue
 * @return pid on success, SYSERR if pid is not found
 */
pid32	remove(pid32 pid, qid16 q)
{	
	//TODO - return EMPTY if queue is empty
	if(isempty(q)) {
		return EMPTY;
	}
	
	//TODO - return SYSERR if pid is illegal
	if(isbadpid(pid) == TRUE) {
		return SYSERR;
	}
	
	else {
		
		//buffers for removal
		struct qentry *entry = getbypid(pid, q);
		struct qentry *buffer = entry.qnext;
		
		//TODO - remove process identified by pid parameter from the queue and return its pid
		entry.qprev.qnext = buffer;
		buffer.qprev = entry.qprev;
		free(entry, sizeof(entry));
		return pid;		
	}
}
