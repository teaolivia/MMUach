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

void chooseVictim()){
	//
	//
	// ALGORITHM START HERE
}

int main(int argc, char*argv[]){
	//---- Scan pada page table untuk menemukan requested field yang diisi oleh MMU
	//---- Jika ditemukan
	//---- Jika terdapat frame yang tidak ditempati
	//---- Jika seluruh frame ditempati, memilih page 'victim'
		// -- page 'victim' memiliki dirty = TRUE', simulasikan penulisan pada disk, 
		// 	  lakukan sleep(1) dan tambahkan nilai pengaksesan pada disk 
		// -- Update page table untuk menyatakan page tersebut sudah tidak berada pada memori fisik
	//---- Simulasikan pemuatan page dengan sleep(1) dan tambahkan nilai pengaksesan pada disk
	//---- update page table
	//---- cetak page table setelah diupdate
	//---- mengirimkan sinyal sigcont ke MMU
	//---- Jika tidak ditemukan field requested dengan nilai 0, berhenti
	return 0;
}
