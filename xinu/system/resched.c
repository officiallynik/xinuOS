/* resched.c - resched, resched_cntl */

#include <xinu.h>

int resched_count;	/* Tracks how many times reschedule is called */

struct	defer	Defer;
pid32 currprio=0;
/*------------------------------------------------------------------------
 *  resched  -  Reschedule processor to highest priority eligible process
 *------------------------------------------------------------------------
 */
void	resched(void)		/* Assumes interrupts are disabled	*/
{
	//Iterate resched_count
	resched_count++;

	//run deadlock detection every 50 times reschedule() is called
	if(resched_count == 50)
	{		
		//disable interrupts
		intmask mask = disable();

		//look for deadlocks
		deadlock_detect();

		//restore interrupts
		restore(mask);
	
		//reset the count
		resched_count = 0;
	}

	struct procent *ptold;	/* Ptr to table entry for old process	*/
	struct procent *ptnew;	/* Ptr to table entry for new process	*/

	/* If rescheduling is deferred, record attempt and return */

	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	/* Point to process table entry for the current (old) process */
//	kprintf("************\n");
	int16 curr=firstid(readylist);
	int flag=1;
	while(curr!=queuetail(readylist)){
		queuetab[curr].qkey+=1;
//		kprintf("Key: %d Name: %s\n",queuetab[curr].qkey,(&proctab[curr])->prname);
		if(queuetab[curr].qkey!=currprio){
			flag=0;
		}
		curr=queuetab[curr].qnext;
	}
	if(flag==1){
		int16 curr=firstid(readylist);
        	while(curr!=queuetail(readylist)){
                	queuetab[curr].qkey=10;
                	curr=queuetab[curr].qnext;
		}
	}

	

	ptold = &proctab[currpid];
	if(flag==1){
		//currprio = (currprio > ptold->prprio)?currprio:ptold->prprio;
	//else
		currprio=10;
	}
	if (ptold->prstate == PR_CURR) {  /* Process remains eligible */
		if (currprio > firstkey(readylist)) {
			return;
		}

		/* Old process will no longer remain current */

		ptold->prstate = PR_READY;
		insert(currpid, readylist, currprio);
	}

	/* Force context switch to highest priority ready process */
	currprio=firstkey(readylist);
	currpid = dequeue(readylist);
	ptnew = &proctab[currpid];
	ptnew->prstate = PR_CURR;
	/* Printing the name of the new process that was scheduled */
//	kprintf("%s\n",ptnew->prname);

	preempt = QUANTUM;		/* Reset time slice for process	*/
	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

	/* Old process returns here when resumed */

	return;
}

/*------------------------------------------------------------------------
 *  resched_cntl  -  Control whether rescheduling is deferred or allowed
 *------------------------------------------------------------------------
 */
status	resched_cntl(		/* Assumes interrupts are disabled	*/
	  int32	defer		/* Either DEFER_START or DEFER_STOP	*/
	)
{
	switch (defer) {

	    case DEFER_START:	/* Handle a deferral request */

		if (Defer.ndefers++ == 0) {
			Defer.attempt = FALSE;
		}
		return OK;

	    case DEFER_STOP:	/* Handle end of deferral */
		if (Defer.ndefers <= 0) {
			return SYSERR;
		}
		if ( (--Defer.ndefers == 0) && Defer.attempt ) {
			resched();
		}
		return OK;

	    default:
		return SYSERR;
	}
}
