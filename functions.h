#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__
#include <stdio.h>
typedef struct KeySpace2{
    int busy;
    char* key;
    char* info;
} KeySpace2;

typedef struct KeySpace2_f{
    int elem_of;
} KeySpace2_f;

typedef struct Table{
    int msize2;
    int csize2;
    char* fname;
    FILE* file;
    KeySpace2_f* ks_f2;
} Table;

typedef struct Table_old{
    int msize2;
    int csize2;
    KeySpace2* ks;
} Table_old;

void showMenu();

int verify(char* menu);
int if_stupid(char* size);
char* verify_name_of_file(char* fname);
char* verify_selected(char* choice);

Table* create_table(int size, char* fname);
Table* get_smth_from_file(char* fname);


int push(char* key, char* inf, Table* ptable);
void show_oldtable(Table_old* ptable);
Table_old* find(Table* ptable, char* key);
int remove_key(Table* ptable, char* key);
Table_old* clean_oldtable(Table_old* ptable);
void show_smth_from_file(Table* ptable);
Table* clean_table(Table* ptable);

#endif //__FUNCTIONS_H__