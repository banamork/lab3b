#ifndef __FUNCT__
#define __FUNCT__
#include <stdio.h>
typedef struct KeySpace2_l{
    int busy;
    char* key;
    char* info;
} KeySpace2_l;

typedef struct KeySpace2_f{
    int element_f;
} KeySpace2_f;

typedef struct Table{
    int size2a;
    int size2b;
    char* filename;
    FILE* file;
    KeySpace2_f* ks_f;
} Table;

typedef struct old_Table{
    int old_size2a;
    int old_size2b;
    KeySpace2_l* ks_l;
} old_Table;

void showMenu();

int verify(char* menu);
int if_stupid(char* size);
char* verify_name_of_file(char* filename);
char* verify_selected(char* selected);

Table* create_table(int size, char* filename);
Table* bookmark_to_file(char* filename);


int push(char* key, char* info, Table* newtable);
void show_oldtable(old_Table* newtable);
old_Table* find(Table* newtable, char* key);
int remove_key(Table* newtable, char* key);
old_Table* clean_oldtable(old_Table* newtable);
void viewer_to_file(Table* newtable);
Table* clean_table(Table* newtable);

#endif //__FUNCT__