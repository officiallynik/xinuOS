#include <xinu.h>

int32 warningType;
pid32 pids[5];
// wait()
// warning=1
// // context switch
// signal()

void warning(sid32 semWarning)
{
    while (1)
    {

        if (warningType == 1)
        {
            kill(pids[0]);
            printf("\f");
            printf("You have crossed the speed limit!!!!!!!!\n");
            printf("Slow Down!!!!\n");
            printf("Speed thrills but kills!!!\n");
        }
        else if (warningType == 2)
        {
            kill(pids[0]);
            printf("\f");
            printf("Your fuel is very low!!!!!!!!\n");
        }
        wait(semWarning);
            warningType = 0;
        signal(semWarning);
            pids[0] = create(dashboard, 2000, 50, "Dashboard", 0);
        resume(pids[0]);
    }

    exit();
}
