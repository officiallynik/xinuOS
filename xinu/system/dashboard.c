#include <xinu.h>


int currentProcessIndex;
pid32 pids[6];
void dashboard()
{
//    printf("PIDS: %d %d %d\n",pids[0],pids[1],pids[2]);
    char ch;
    char names[4][20] = {"clock", "calendar","speed","fuel"};
    int flag = 0;
    pid32 chosenProcess;
    while(!flag){
        printf("List of apps to navigate:-\n");
        int i;
	for(i=0;i<4;i++){
            printf("%d: %s\n", i, names[i]);
        }
        printf("Type one of above commands and press enter!\n");
        char str[100] = "";
        int j = 0;
        while((ch=getchar())!='\n'){
            str[j++] = ch;
        }
	//printf("%s\n",str);
        for(i=0;i<4;i++){
            //printf("%d: %s\n", i, names[i]);
            if(strcmp(names[i], str)==0){
                printf("%s activates pid: %d\n", names[i],pids[i+1]);
                chosenProcess=pids[i+1];
		currentProcessIndex=i+1;
		flag = 1;
                break;
            }
        }
        if(!flag) printf("No such command found!\n");
    }
	resume(chosenProcess);
}


