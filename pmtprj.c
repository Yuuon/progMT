#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>
#define EASY 10
#define NORMAL 5
#define HARD 3
#define TIME_EASY 120
#define TIME_NORMAL 60
#define TIME_HARD 30

int i=0,t=1;

struct Timer{
	int total_time;
	int left_time;
	int func;
}myTimer[N];

void setTimer(int t,int f){
	struct Timer a;
	a.total_time=t;
	a.left_time=t;
	a.func=f;
	myTimer[i++]=a;
}

void timeout(){
	printf("Time: %d\n",t++);
	int j;
	for(j=0;j<i;j++){
		if(myTimer[j].left_time!=0)
			myTimer[j].left_time--;
		else{
			switch(myTimer[j].func){
				case 1: {
					printf("Time's UP!");
					break;
				}
			}
		myTimer[j].left_time=myTimer[j].total_time;
		}
	}
}

int NB_fuyant(int NB_SECRET){
	srand((int)time(0));
	switch(rand()%3){
		case 1:{NB_SECRET++;break;}
		case 2:{NB_SECRET--;break;}
		default: return NB_SECRET;
	}
	return NB_SECRET;
}

int main(){
	int a,b;
	srand((int)time(0));
	a=rand()%100;
	do{
		b=rand()%100;
	}while(abs(a-b)<10);
	if(a>b) {a=a+b;b=a-b;a=a-b;};
	int NB_SECRET;
	do{
		NB_SECRET=rand()%b;
	}while(NB_SECRET<a);
	
	int c;
	do{
		printf("Choose difficulty:\n");
		printf("1.EASY 2.NORMAL 3.HARD\n");
		scanf("%d",&c);
	}while(c!=1 && c!=2 && c!=3);
	int NB_prop,NB_guess,TIME;
	switch(c){
		case 1: {NB_prop=EASY;TIME=TIME_EASY;break;}
		case 2: {NB_prop=NORMAL;TIME=TIME_NORMAL;break;}
		default: {NB_prop=HARD;TIME=TIME_HARD;}
	}
	printf("The secret number is between %d and %d!",a,b);
	setTimer(TIME,1);
	do{
		int tube[2];
		pipe(tube);
		switch(fork()){
		case -1:{
			perror("error in create processus");
			exit(99);
		}
		case 1:{
			close(tube[0]);
			write(tube[1],getpid(),4);
			if(NB_prop>1){
				printf("You have %d tries left\n",NB_prop);
			}else{printf("You have only 1 try left\n");}
			do{
				printf("Please enter the number you guess:");
				scanf("%d",&NB_guess);
			}while(NB_guess<a || NB_guess>b);
			if (NB_SECRET==NB_guess) {
				printf("Congratulations!You find the secret number!\n");
				break;
			}else if (NB_SECRET>NB_guess){
				printf("The secret number is bigger!And may changes a little~\n");
				NB_SECRET=NB_fuyant(NB_SECRET);
			}else {
				printf("The secret number is smaller!And may changes a little~\n");
				NB_SECRET=NB_fuyant(NB_SECRET);
			}
			NB_prop--;
		}while(NB_prop>0); 
		break;
		}
		case 0:{
			int ppid;
			close(tube[1]);
			read(tube[0],ppid,4);
			setTimer(TIME,1);
			signal(SIGALRM,timeout());
			while(1){
				sleep(1);
				kill(ppid,SIGALRM);
			}
		}
	if(NB_prop==0){
		printf("You failed!The secret number is:%d\n",NB_SECRET);
	}
	return 0;
}


