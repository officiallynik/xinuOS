#include <xinu.h>

int currentSpeed;
int averageSpeed;
pid32 pids[6];
int warningType;
int currentProcessIndex;

int speedgen(int speed)
{

    int lower = speed - 5;      // lower bound to find random speed
    int upper = speed + 5;     // upper bound to find random speed

    // Edge case optimisation
    lower = lower > 0 ? lower : 0;
    upper = upper < MAX_SPEED ? upper : MAX_SPEED;

    int range = 11;

    // New speed calculation
    int speedChange = (rand() % (range));
//    printf("Speed Change: %d\n",speedChange);
    int newspeed=speed;
    if(speedChange<=5)
	newspeed=newspeed-speedChange;
    else{
	newspeed=newspeed+speedChange-5;
    }
    newspeed = newspeed > 0 ? newspeed : 0;
    newspeed = newspeed < MAX_SPEED ? newspeed : MAX_SPEED;
    return newspeed;
}


void getspeed(sid32 semWarning)
{
	int speed = 60;
	currentSpeed = 60;
	while (1)
	{
		averageSpeed = updatespeedbuffer(currentSpeed);
		speed = speedgen(speed);
		currentSpeed = speed;
		sleep(1);
		if(currentSpeed>=70){
			kill(pids[currentProcessIndex]);
			kill(pids[0]);
			//printf("Killed\n");
			wait(semWarning);
			warningType = 1;
			//printf("Warning!!\n");
			signal(semWarning);
			//printf("warning setup 1\n");
			speed=0;
			currentSpeed=0;
			if(currentProcessIndex==3)
				pids[3]=create(printSpeed, 2000, 30, "Print Speed", 0);
			else if(currentProcessIndex==4)
				pids[4]=create(printFuel,2000,30,"Print Fuel",0);
		}
	}
}


void printSpeed(){

	kill(pids[0]);
	printf("\f");
	int i=0;
	//printf("Current Speed: ");
	while(1){
		printf("Current Speed: %02d    Average Speed: %02d", currentSpeed, averageSpeed);
		printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
		sleep(1);
		i++;
		if(i==15)
		{
			printf("\f");
			printf("To continue type c and press enter!\n");        
			printf("To exit type e and press enter!\n");
			char option;
			option=getchar();
			char newline=getchar();
			printf("\f");
			if(option=='e'){
					break;
			}
			i=0;
		}
	}
	pids[0]=create(dashboard,2000,50,"Dashboard",0);
	pids[3]=create(printSpeed, 2000, 30, "Print Speed", 0);
	resume(pids[0]);
	exit();




}
