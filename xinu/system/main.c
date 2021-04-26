/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>

#define N 4

lid32	printer_lock;
lid32	mylock[N];


/**
 * Delay for a random amount of time
 * @param alpha delay factor
 */
void	holdup(int32 alpha)
{
	long delay = rand() * alpha;
	while (delay-- > 0)
		;	//no op
}

/**
 * Work for a random amount of time
 * @param id ID of worker
 */
void	work(uint32 id)
{
	acquire(printer_lock);
	kprintf("Worker %d: pid=%d\n", id, currpid);
	release(printer_lock);
	holdup(10000);
}


/**
 * Worker code
 * @param id ID of worker
 */
void	worker(uint32 id)
{
	if (id == 0)
	{
		acquire(mylock[0]);
		work(id);
		acquire(mylock[3]);
		work(id);
		release(mylock[3]);
		release(mylock[0]);
	}
	else
	{
		acquire(mylock[id]);
		work(id);
		acquire(mylock[0]);
		work(id);
		release(mylock[0]);
		release(mylock[id]);
	}
}

process	main(void)
{
	/* Run the Xinu shell */

	recvclr();
	int i;
	printer_lock = lock_create();
	for (i=0; i<N; i++)
		mylock[i] = lock_create();

	resume(create(worker, 8192, 15, "w0", 1, 0));
	resume(create(worker, 8192, 15, "w1", 1, 1));
	resume(create(worker, 8192, 15, "w2", 1, 2));
	resume(create(worker, 8192, 15, "w3", 1, 3));
	resume(create(worker, 8192, 15, "w4", 1, 4));

	/* Wait for shell to exit and recreate it */

	// while (TRUE) {
	//	receive();
	//	sleepms(200);
	//	kprintf("\n\nMain process recreating shell\n\n");
	//	resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	// }
	return OK;
}
