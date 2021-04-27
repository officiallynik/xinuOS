#include <xinu.h>

local	lid32	newlock(void);

/**
 * Create a new semaphore and return the ID to the caller
 * @return ID of the mutex to caller, or SYSERR
 */
lid32	lock_create()
{
	intmask	mask;		/* saved interrupt mask	*/
	lid32	lockid;		/* lock ID to return	*/

	mask = disable();

	if ((lockid = newlock()) == SYSERR)
	{
		restore(mask);
		return SYSERR;
	}

	restore(mask);
	return lockid;
}

/**
 * Allocate an unused lock and return its index
 * @return	ID of free lock
 */
local	lid32	newlock(void)
{
	static	lid32	nextlock = 0;	/* next lockid to try	*/
	lid32	lockid;			/* ID to return	*/
	int32	i;			/* iterate through # entries	*/
	struct 	lockentry *lptr;	/* ptr for lock table entry */

	// START

	// loop through each element in the lock table.
	for(i = 0; i < NLOCK; i++)
	{
		lptr = &locktab[i];
		
		// and find a lock that is free to use
		if(lptr->state == LOCK_FREE)
		{

		
			// set its state to used, and reset the mutex to FALSE
			lptr->state = LOCK_USED;
			lptr->lock = FALSE;

			// return its lockid (index in table)
			lockid = i;
			return lockid;
		}	
	}
		
	// if there is no such lock, return SYSERR
	return SYSERR;
}


/**
 * Delete a lock by releasing its table entry
 * @param ID of lock to release
 */
syscall	lock_delete(lid32 lockid)
{
	intmask mask;			/* saved interrupt mask		*/
	struct	lockentry *lptr;	/* ptr to lock table entry	*/

	mask = disable();
	if (isbadlock(lockid)) {
		restore(mask);
		return SYSERR;
	}

	lptr = &locktab[lockid];
	if (lptr->state == LOCK_FREE) {
		restore(mask);
		return SYSERR;
	}

	// START

	// reset lock's state to free and the mutex to FALSE
	lptr->state = LOCK_FREE;
	lptr->lock = FALSE;

	// remove all processes waiting on its queue, and send them to the ready queue
	int i;
	pid32 dequeue_pid;
	pid32 enqueue_pid;

	for(i = 0; i < lptr->wait_queue->lqsize; i++)
	{
		lptr = &locktab[i];
		dequeue_pid = ldequeue(lptr->wait_queue);
		enqueue_pid = READY(dequeue_pid, 0);
	}
	
	

	// (RAG) remove all RAG edges to and from this lock
	for(i = 0; i < NPROC; i++)
	{
		rag_dealloc(i, lockid);
	}
		
	
	// END

	resched();
	restore(mask);
	return OK;
}


/**
 * Acquires the given lock, or cause current process to wait until acquired
 * @param lockid	lock on which to wait
 * @return status
 */
syscall	acquire(lid32 lockid)
{
	intmask mask;			// saved interrupt mask
	struct	lockentry *lptr;	// ptr to sempahore table entry

	mask = disable();
	if (isbadlock(lockid)) {
		restore(mask);
		return SYSERR;
	}

	lptr = &locktab[lockid];
	if (lptr->state == LOCK_FREE) {
		restore(mask);
		return SYSERR;
	}

	// START
	// enqueue the current process ID on the lock's wait queue
	struct procent *ptnew = &proctab[currpid];
	pid32 enqueue_pid = lenqueue(currpid, lptr->wait_queue, ptnew->prprio);
 
	// (RAG) add a request edge in the RAG
	rag_request(currpid, lockid);
	
	// END

	restore(mask);				//reenable interrupts

	// START
	// lock the mutex!
	mutex_lock(&lptr->lock);
	
	// END

	mask = disable();			//disable interrupts

	// START
	// (RAG) we reache this point. Must've gotten the lock! Transform request edge to allocation edge
	rag_alloc(currpid, lockid);	
	
	// END

	restore(mask);				//reenable interrupts
	return OK;
}

/**
 * Unlock, releasing a process if one is waiting
 * @param lockid ID of lock to unlock
 * @return status
 */
syscall	release(lid32 lockid)
{
	int32 i, j;
	intmask mask;			/* saved interrupt mask		*/
	struct	lockentry *lptr;	/* ptr to lock table entry	*/

	mask = disable();
	if (isbadlock(lockid)) {
		restore(mask);
		return SYSERR;
	}
	lptr= &locktab[lockid];
	if (lptr->state == LOCK_FREE) {
		restore(mask);
		return SYSERR;
	}

	// START
	// remove current process' ID from the lock's queue
	pid32 remove_pid = remove(currpid, lptr->wait_queue);

	// unlock the mutex
	mutex_unlock(&lptr->lock);

	// (RAG) remove allocation edge from RAG
	rag_dealloc(remove_pid, lockid);
	
	// END

	restore(mask);
	return OK;
}

