#include <xinu.h>

int warningType;
pid32 pids[6];

// wait()
// warning=1
// // context switch
// signal()

void warning(sid32 semWarning)
{
	int flag=0;
    while (1)
    {

        if (warningType == 1)
        {
            kill(pids[0]);
            printf("\f");
            printf("You have crossed the speed limit!!!!!!!!\n");
            printf("Slow Down!!!!\n");
            printf("Speed thrills but kills!!!\n");
	    sleep(10);
	    flag=1;
        }
        else if (warningType == 2)
        {
            kill(pids[0]);
            printf("\f");
            printf("Your fuel is very low!!!!!!!!\n");
            sleep(10);
	    flag=1;
	}
	if(warningType!=0 && flag==1){
		printf("\f");
//		printf("warning !0\n");
        	wait(semWarning);
            	warningType = 0;
        	signal(semWarning);
		flag=0;
            	pids[0] = create(dashboard, 2000, 50, "Dashboard", 0);
        	resume(pids[0]);
	}
    }

    exit();
}
