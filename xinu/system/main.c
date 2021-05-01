/*  main.c  - main */

#include <xinu.h>
pid32 pids[6];
int currentFuel;

void infinite_loop(char c){
	//while(1){
	kill(pids[0]);
	printf("Process: %c\n",c);
//}
//	sleep(10);
	pids[0]=create(dashboard,2000,50,"Dashboard",0);

//	printf("Process Completed\n");
	if(c=='a'){
	pids[1]=create(infinite_loop, 2000, 30, "proc A", 1,'a');
}else{
	pids[2]=create(infinite_loop, 2000, 30, "proc A", 1,'b');
}
	resume(pids[0]);
	exit();
}

void getCal()
{
	kill(pids[0]);
	int year = 2021;
	int date = 13;
	int month = 4;
	printCalendar(year, month, date);
	pids[0]=create(dashboard,2000,50,"Dashboard",0);
	pids[2]=create(getCal, 2000, 30,"Calendar", 0);
	resume(pids[0]);
        exit();
}

void printTime()
{
	kill(pids[0]);
	int days = 0, hours = 0, minutes = 0, seconds = 0;
//	printf("Clock Time: ");
	int i=0;
	printf("\f");
	while (1)
	{
		days = clktime / (24 * 60 * 60);
		int time_in_day = clktime % (24 * 60 * 60);
		hours = time_in_day / (60 * 60);
		int minutes_in_day = time_in_day % (60 * 60);
		minutes = minutes_in_day / 60;
		seconds = minutes_in_day % 60;

		printf("days:%02d HH:%02d MM:%02d SS:%02d", days, hours, minutes, seconds);
		lfflush(stdout);
		printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
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
	pids[1]=create(printTime, 2000, 30,"Clock", 0);
        resume(pids[0]);
        exit();
}





process	main(void)
{
	/* Run the Xinu shell */

	// Initialize memory buffer pool
	bufferinit();

	// Further processes
	printf("Welcome to our infotainment system!!\n");
	printf("Enter current fuel value\n");
	char c = getchar();
	int num = 0;
	while (isdigit(c)){
		num = (num * 10) + (c - '0');
		c = getchar();
	}
	currentFuel=num;
	printf("Current Fuel: %d\n",num);
	sid32 semWarning=semcreate(1);

	pid32 prClock = create(printTime, 2000, 30, "Clock", 0);
	pid32 prB = create(getspeed, 2000, 30, "proc Speed", 1 ,semWarning);
	pid32 prCalendar = create(getCal, 2000, 30, "Calendar", 0);
	pid32 prPrintSpeed = create(printSpeed, 2000, 30, "Print Speed", 0);
	pid32 prWarning = create(warning,2000,30,"Warning",1,semWarning);
	pid32 prFuelGen= create(getFuel,2000,30,"proc Fuel",0);
	pid32 prPrintFuel = create(printFuel, 2000,30,"Print Fuel",0);
	//resume(prA);
	resume(prB);
	//resume(prC);
	resume(prFuelGen);
	resume(prWarning);

	pids[1]=prClock;
	pids[2]=prCalendar;
	pids[3]=prPrintSpeed;
	pids[5]=prWarning;
	pids[4]=prPrintFuel;
	pid32 prDash = create(dashboard,2000,50,"Dashboard",0);
	pids[0]=prDash;
	//printf("Pids: %d %d %d\n",pids[0],pids[1],pids[2]);
	resume(prDash);
	//kill(prA);
	//kill(prB);
	//kill(prC);
	return OK;
    
}
