/*  main.c  - main */

#include <xinu.h>
#include <stdlib.h>
#include <stdio.h>

void getavgspeed()
{
	bufferinit();
	int speed = 0;
	int testcases = 10;
	float avg;
	while (testcases--)
	{
		avg = updatespeed(speed);
		printf("Speed : %d ; Average Speed : %.2f\n", speed, avg);
		speed++;
	}
}

process	main(void)
{
	/* Run the Xinu shell */

	recvclr();
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));
	resume(create(getavgspeed, 8192, 30, "Average", 1, CONSOLE));

	/* Wait for shell to exit and recreate it */

	// while (TRUE) {
	// 	receive();
	// 	sleepms(200);
	// 	kprintf("\n\nMain process recreating shell\n\n");
	// 	resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	// }
	return OK;
    
}
