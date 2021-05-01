/* queue.c - enqueue, dequeue, isempty, nonempty, et al. */

#include <xinu.h>
#include <stdlib.h>

/**
 * Size of the queue
 *
 * @param q pointer to a queue
 * @return int32 size of queue
 */
int32 	lsize(struct lqueue *q)
{
	return q->lqsize;
}

/**
 * Prints out contents of a queue
 * @param q	pointer to a queue
 */
void	printlqueue(struct lqueue *q)
{	
	struct lqentry *node = q->head;
	kprintf("\n");
	
	while (node != NULL)
	{

		//if there is only 1 element
		if(q->head == node && q->tail == node)
		{
			kprintf("[(pid=%d, key=%d)]. ", node->pid, node->key);
		}
		
		//1st process
		else if(q->head == node) 
		{
			kprintf("[(pid=%d, key=%d), ", node->pid, node->key);
		}
		
		//last process
		else if(q->tail == node)
		{
			kprintf("(pid=%d, key=%d)].\n", node->pid, node->key);	
		}
		
		//every other process
		else 
		{
			kprintf("(pid=%d, key=%d), ", node->pid, node->key);
		}

		node = node->next;
	}
	
	kprintf("\n");
}

/**
 * Checks whether queue is empty
 * @param q	Pointer to a queue
 * @return TRUE if true, FALSE otherwise
 */
bool8	lisempty(struct lqueue *q)
{
	//TODO
	if(q->head == NULL) {
		return TRUE;
	}
	
	return FALSE;
}

/**
 * Checks whether queue is nonempty
 * @param q	Pointer to a queue
 * @return TRUE if true, FALSE otherwise
 */
bool8	lnonempty(struct lqueue *q)
{
	// don't just check q's size because q could be NULL
	if(lisempty(q) == TRUE) {
		return FALSE;
	}
	
	return TRUE;
}


/**
 * Checks whether queue is full
 * @param q	Pointer to a queue
 * @return TRUE if true, FALSE otherwise
 */
bool8	lisfull(struct lqueue *q)
{
	// check if there are at least NPROC processes in the queue
	if(lsize(q) >= NPROC) {
		return TRUE;
	}

	return FALSE;
}


/**
 * Insert a process at the tail of a queue
 * @param pid	ID process to insert
 * @param q	Pointer to the queue to use
 * @param key	priority key of process
 *
 * @return pid on success, SYSERR otherwise
 */
pid32 lenqueue(pid32 pid, struct lqueue *q, int32 key)
{
        // check if queue is full and if pid is illegal, and return SYSERR if either is true
	if(lisfull(q) == TRUE || isbadpid(pid) == TRUE) {
		return SYSERR;
	}
        
	// allocate space on heap for a new lQEntry
	struct lqentry *newentry = (struct lqentry*) malloc(sizeof(struct lqentry));

        // initialize the new lQEntry
	newentry->pid = pid;
	newentry->key = key;		

	if(lisempty(q) == TRUE) {
		newentry->prev = NULL;
		newentry->next = NULL;
		q->head = newentry;
		q->tail = newentry;
	}
	
	else {
		if(key > q->head->key)
		{
			//kprintf("First element here has priority %d\n", key);
			newentry->next = q->head;
			newentry->prev = NULL;
			q->head = newentry;
			newentry->next->prev = newentry;
			
		}
		
		else 
		{
			struct lqentry *iter = q->head;
			int cont = 1;
	
			//iterate throught the queue
			while(iter != NULL && cont == 1) {
				
				//regular case
				if(iter->key < key) {
					//kprintf("%d is greater than %d\n", key, iter->key);
					newentry->next = iter;
					newentry->prev = iter->prev;
					newentry->prev->next = newentry;
					iter->prev = newentry;
					cont = 0;
				}

				//the tail
				else if(iter->next == NULL)
				{
					newentry->prev = q->tail;	
					newentry->next = NULL;
					q->tail->next = newentry;
					q->tail = newentry;
					cont = 0;
				}
				
				// find the lqentry with the given pid
				else {
					//kprintf("%d is NOT greater than %d\n", key, iter->key);
					iter = iter->next;
				}
			}
		}
	}

        // return the pid on success
	//printqueue(q);
	q->lqsize++;	
	return pid;
}


/**
 * Remove and return the first process on a list
 * @param q	Pointer to the queue to use
 * @return pid of the process removed, or EMPTY if queue is empty
 */
pid32 ldequeue(struct lqueue *q)
{
	pid32 xpid = 0;

        // return EMPTY if queue is empty
	if(lisempty(q) == TRUE) {
		kprintf("\nIs empty on dequeue\n");
		return EMPTY;
	}

	else {
	        // get the head entry of the queue
		struct lqentry *swapper = q->head;
		xpid = q->head->pid;
		
		// unlink the head entry from the rest
		q->head = q->head->next;

		// free up the space on the heap
		q->lqsize--;
		freemem(swapper, sizeof(swapper));
	}	

       
	 // return the pid on success
	//printqueue(q);
	return xpid;
}


/**
 * Finds a queue entry given pid
 * @param pid	a process ID
 * @param q	a pointer to a queue
 * @return pointer to the entry if found, NULL otherwise
 */
struct lqentry *lgetbypid(pid32 pid, struct lqueue *q)
{
	// return NULL if queue is empty or if an illegal pid is given
	if(lisempty(q) == TRUE || isbadpid(pid) == TRUE) {
		return NULL;
	}

	struct lqentry *iter = q->head;
	
	//if first element
	while(iter != NULL) {
		
		if(iter->pid == pid) {
			return iter;
		}

		// find the lqentry with the given pid
		else {
			iter = iter->next;
		}
	}

	return NULL;
}

/**
 * Remove a process from the front of a queue (pid assumed valid with no check)
 * @param q	pointer to a queue
 * @return pid on success, EMPTY if queue is empty
 */
pid32	lgetfirst(struct lqueue *q)
{
	// return EMPTY if queue is empty
	if(lisempty(q)) {
		return EMPTY;
	}

	// remove process from head of queue and return its pid
	else {
		return ldequeue(q);
	}
}

/**
 * Remove a process from the end of a queue (pid assumed valid with no check)
 * @param q	Pointer to the queue
 * @return pid on success, EMPTY otherwise
 */
pid32	lgetlast(struct lqueue *q)
{
	// return EMPTY if queue is empty
	if(lisempty(q)) {
		return EMPTY;
	}
	
	pid32 pid = q->tail->pid;
	
	return lremove(pid, q);
}

/**
 * Remove a process from an arbitrary point in a queue
 * @param pid	ID of process to remove
 * @param q	Pointer to the queue
 * @return pid on success, SYSERR if pid is not found
 */
pid32	lremove(pid32 pid, struct lqueue *q)
{	
	// return EMPTY if queue is empty
	if(lisempty(q)) {
		return EMPTY;
	}
	
	// return SYSERR if pid is illegal
	if(isbadpid(pid) == TRUE) {
		return SYSERR;
	}
	
	else {
		
		//buffers for removal
		struct lqentry *entry = lgetbypid(pid, q);
		struct lqentry *buffer = entry->next;
		
		// remove process identified by pid parameter from the queue and return its pid
		entry->prev->next = buffer;
		buffer->prev = entry->prev;
		freemem(entry, sizeof(entry));
		return pid;		
	}
}
