#include <xinu.h>

int currentFuel;
pid32 pids[5];
int warningType;
int currentProcessIndex;
int currentSpeed;

float fuelgen(float fuel){

    float mileage = 50;

   
        float dist = currentSpeed*0.278;
        fuel -= (dist/1000.000)/mileage;
        //printf("fuel: %.3f\n", fuel);
        //sleep(1);
    return fuel;
}


void getFuel(sid32 semWarning)
{
	int i=0;
	int fuel = currentFuel;
	while (1)
	{
		if(i%7==0){
			fuel = fuelgen(fuel);
			currentFuel = fuel;
		}
		sleep(1);
		i++;
		if(currentFuel<=10){
			kill(pids[currentProcessIndex]);
			kill(pids[0]);
			//printf("Killed\n");
			wait(semWarning);
            			warningType = 2;
				//printf("Warning!!\n");
        		signal(semWarning);
			//printf("warning setup 1\n");
			fuel=50;
			currentFuel=50;
			if(currentProcessIndex==3)
				pids[3]=create(printSpeed, 2000, 30, "Print Speed", 0);
			else if(currentProcessIndex==4)
				pids[4]=create(printFuel, 2000, 30, "Print Fuel", 0);
	}
}
}
void printFuel(){

	kill(pids[0]);
	printf("\f");
	int i=0;
	//printf("Current Speed: ");
	while(1){
		printf("Current Fuel: ");
		printf("%02d",currentFuel);
		printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
		sleep(1);
		i++;
		if(i==15){
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
	pids[4]=create(printFuel, 2000, 30, "Print Fuel", 0);
	resume(pids[0]);
	exit();




}

