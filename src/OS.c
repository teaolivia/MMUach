#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include <errno.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include "PageTable.h"


typedef struct OS_t {
	int shmid, OSID, frame, page, MMid, disk, reqpage, finished;
	page_table_pointer pagetable;	
} OS_t;

OS_t* os = NULL;

void initOs(int page, int frame){
	os = malloc(sizeof(OS_t));

	os->page = page;
	os->frame = frame;
	os->finished = 0;
	os->disk = 0;
	os->OSID = getpid();
	os->shmid = shmget(os->OSID,os->page*sizeof(page_table_entry),IPC_CREAT|0666);
	os->pagetable = shmat(os->shmid,0,0);

	// init page table
	int i;
	for(i=0; i<page; i++){
		os->pagetable[i].Valid = 0;
		os->pagetable[i].Frame = -1;
		os->pagetable[i].Dirty = 0;
		os->pagetable[i].Requested = 0;
	}
}

void PrintPageTable(page_table_entry PageTable[],int NumberOfPages) {

    int Index;

    for (Index =  0;Index < NumberOfPages;Index++) {
        printf("%2d: Valid=%1d Frame=%2d Dirty=%1d Requested=%1d\n",Index,
PageTable[Index].Valid,PageTable[Index].Frame,PageTable[Index].Dirty,
PageTable[Index].Requested);
    }

}


void continueExec(int p){
	int MMid = 0;
	//scan pada page table untuk menemukan field Requested
	int i=0;
	int reqpage;
	while(i<os->page){
		if(os->pagetable[i].Requested!=0){
			MMid = os->pagetable[i].Requested;
			reqpage = i;
			break;
		} else {
			i++;
		}
	}
	
	if(MMid==0){
		printf("The MMU has finished\n");
		PrintPageTable(os->pagetable,os->page);
		//Clear Shm
		//Count disk access
		printf("%d disk accesses required\n",os->disk);
		os->finished = 1;
	} else {
		printf("Process %d has requested page %d\n",MMid,reqpage);
		//count frame
		int i;
		int count=0;
		for(i=0; i<os->page; i++){
			if(os->pagetable[i].Frame!=-1){
				count++;
			}
		}
		
		//Check Frame
		if(count>=os->frame){
			//frame is full, swap
			
			
			//Search Frame
			int i=0;
			while(os->pagetable[i].Frame==-1){
				i++;
			}
			printf("Chose a victim page %d\n",i);
			
			
			//check if it is dirty
			if(os->pagetable[i].Dirty!=0){
				printf("Victim is dirty, write out\n");
				sleep(1);
				os->pagetable[i].Dirty = 0;
				printf("Put in victim's frame %d\n",os->pagetable[i].Frame);
				os->disk++;
			}
			os->pagetable[reqpage].Frame = os->pagetable[i].Frame;
			os->pagetable[reqpage].Valid = 1;
			os->pagetable[reqpage].Requested = 0;
			
			os->pagetable[i].Valid = 0;
			os->pagetable[i].Frame = -1;
			os->pagetable[i].Requested = 0;
			
			
			os->disk++;
			kill(MMid,SIGCONT);
		} else {
			//allocate to some frame
			
			os->pagetable[reqpage].Frame = count;
			os->pagetable[reqpage].Valid = 1;
			os->pagetable[reqpage].Requested = 0;
			printf("Put in free frame %d\n",count);
			printf("Unblock MMU\n");
			os->disk++;
			kill(MMid,SIGCONT);
		}
	}
}

int main(int argc,char *argv[]){
	initOs(atoi(argv[1]), atoi(argv[2]) );
	signal(SIGUSR1,continueExec);
	printf("The shared memory key (PID) is %d\n", os->OSID);
	
	while(!os->finished);
	return 0;
}
