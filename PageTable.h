//PageTable.h

typedef struct {
int Valid; // apakah page terdapat pada memori fisik
int Frame; // indeks frame page di memori fisik
int Dirty; // apakah page tersebut telah ditulis
int Requested;
} page_table_entry;

typedef page_table_entry* page_table_pointer;