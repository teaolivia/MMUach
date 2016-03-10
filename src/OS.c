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

void chooseVictim(){
	//
	//
	// ALGORITHM START HERE
}

int shmid, OSID, frame, page, MMid, disk, reqpage, finished;
page_table_pointer pagetable;

void PrintPageTable(page_table_entry PageTable[],int NumberOfPages) {

    int Index;

    for (Index =  0;Index < NumberOfPages;Index++) {
        printf("%2d: Valid=%1d Frame=%2d Dirty=%1d Requested=%1d\n",Index,
PageTable[Index].Valid,PageTable[Index].Frame,PageTable[Index].Dirty,
PageTable[Index].Requested);
    }

}

void init(int p){
	int i;
	for(i=0; i<p; i++){
		pagetable[i].Valid = 0;
		pagetable[i].Frame = -1;
		pagetable[i].Dirty = 0;
		pagetable[i].Requested = 0;
	}
}

void continueExec(){
	MMid = 0;
	//scan pada page table untuk menemukan field Requested
	int i=0;
	while(i<page){
		if(pagetable[i].Requested!=0){
			MMid = pagetable[i].Requested;
			reqpage = i;
			break;
		} else {
			i++;
		}
	}
	
	if(MMid==0){
		printf("The MMU has finished\n");
		PrintPageTable(pagetable,page);
		//Clear Shm
		//Count disk access
		printf("%d disk accesses required\n",disk);
		finished = 1;
	} else {
		printf("Process %d has requested page %d\n",MMid,reqpage);
		//count frame
		int i;
		int count=0;
		for(i=0; i<page; i++){
			if(pagetable[i].Frame!=-1){
				count++;
			}
		}
		
		//Check Frame
		if(count>=frame){
			//frame is full, swap
			
			
			//Search Frame
			int i=0;
			while(pagetable[i].Frame==-1){
				i++;
			}
			printf("Chose a victim page %d\n",i);
			
			
			//check if it is dirty
			if(pagetable[i].Dirty!=0){
				printf("Victim is dirty, write out\n");
				sleep(1);
				pagetable[i].Dirty = 0;
				printf("Put in victim's frame %d\n",pagetable[i].Frame);
				disk++;
			}
			pagetable[reqpage].Frame = pagetable[i].Frame;
			pagetable[reqpage].Valid = 1;
			pagetable[reqpage].Requested = 0;
			
			pagetable[i].Valid = 0;
			pagetable[i].Frame = -1;
			pagetable[i].Requested = 0;
			
			
			disk++;
			kill(MMid,SIGCONT);
		} else {
			//allocate to some frame
			
			pagetable[reqpage].Frame = count;
			pagetable[reqpage].Valid = 1;
			pagetable[reqpage].Requested = 0;
			printf("Put in free frame %d\n",count);
			printf("Unblock MMU\n");
			disk++;
			kill(MMid,SIGCONT);
		}
	}
}

int main(int argc,char *argv[]){
	finished = 0;
	disk = 0;
	signal(SIGUSR1,continueExec);
	page = atoi(argv[1]);
	frame = atoi(argv[2]);
	OSID = getpid();
	printf("The shared memory key (PID) is %d\n",OSID);
	shmid = shmget(OSID,page*sizeof(page_table_entry),IPC_CREAT|0666);
	pagetable = shmat(shmid,0,0);
	init(page);
	
	while(finished==0){
		
	}
	return 0;
}
