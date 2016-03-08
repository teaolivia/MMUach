//PageTable.h
#ifndef _PageTable_H
#define _PageTable_H

#include <stdio.h>
#include <stdlib.h>

typedef struct page_table {
	int Valid; // apakah page terdapat pada physical memory
	int Frame; // indeks frame page di physical memory
	int Dirty; // apakah page tersebut telah ditulis
	int Requested; // != 0 jika tidak ada pada physical memory dan dipesan oleh MMU (Process ID)
} page_table_entry;

typedef page_table_entry* page_table_pointer;

#endif