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

void printMenu();

int check(char* menu);
int check_if_you_an_idiot(char* size);
char* check_file_name(char* fname);
char* check_choice(char* choice);

Table* make_table(int size, char* fname);
Table* get_from_file(char* fname);


int insert(char* key, char* inf, Table* ptable);
void print_table(Table_old* ptable);
Table_old* search(Table* ptable, char* key);
int delete_k(Table* ptable, char* key);
Table_old* indiv_search(Table* ptable, char* key);
Table_old* free_table(Table_old* ptable);
void print_from_file(Table* ptable);
Table* free_ftable(Table* ptable);

#endif //__FUNCTIONS_H__