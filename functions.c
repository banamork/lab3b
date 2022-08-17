#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <readline/readline.h>
#include <ctype.h>
#include "functions.h"
void printMenu(){
    printf("\n =========================================================================== \n");
    printf("Выберите действие: \n");
    printf("0 - Выйти из программы.\n");
    printf("1 - Вставить элемент в таблицу\n");
    printf("2 - Поиск в таблице элемента, заданного составным ключом\n");
    printf("3 - Удаление из таблицы элемента, заданного составным ключом\n");
    printf("4 - Вывод содержимого таблицы на экран\n");
    printf("\n =========================================================================== \n");
}

int check(char* menu){
    while(1){
        if(menu == NULL){
            printf("Вы ничего не ввели. Попробуйте ещё раз.\n");
            menu = readline("");
        }else if(strlen(menu) == 0){
            printf("Вы ничего не ввели. Попробуйте ещё раз.\n");
            free(menu);
            menu = readline("");
        }else if(strlen(menu) == 1){
            if(isdigit(menu[0])){
                int m = atoi(menu);
                free(menu);
                return m;
            }else{
                printf("Неверно введён пункт меню. Попробуйте ещё раз.\n");
                free(menu);
                menu = readline("");
            }
        }else{
            printf("Неверно введён пункт меню. Попробуйте ещё раз.\n");
            free(menu);
            menu = readline("");
        }
    }
}

int check_if_you_an_idiot(char* size){
    while(1){
        if(size == NULL){
            printf("Вы ничего не ввели. Попробуйте ещё раз.\n");
            size = readline("");
        }else if(strlen(size) == 0){
            printf("Вы ничего не ввели. Попробуйте ещё раз.\n");
            size = readline("");
        }else{
            int l = strlen(size);
            int i = 0;
            for(i = 0; i < l; i++){
                if(!(isdigit(size[i]))){
                    break;
                }
            }
            if(i == l){
                int s = atoi(size);
                free(size);
                return s;
            }else{
                printf("Невено введён размер. Попробуйте ещё раз.\n");
                free(size);
                size = readline("");
            }
        }
    }
}

char* check_file_name(char* fname){
    while(1){
        if(fname == NULL){
            printf("Вы ничего не ввели. Попробуйте ещё раз.\n");
            fname = readline("");
        }else if(strlen(fname) == 0){
            free(fname);
            printf("Вы ничего не ввели. Попробуйте ещё раз.\n");
            fname = readline("");
        }else{
            break;
        }
    }
    return fname;
}

char* check_choice(char* choice){
    while(1){
        if(choice == NULL){
            printf("Вы ничего не ввели. Попробуйте ещё раз.\n");
            choice = readline("");
        }else if(strlen(choice) == 0){
            printf("Вы ничего не ввели. Попробуйте ещё раз.\n");
            free(choice);
            choice = readline("");
        }else if(((choice[0] == '1') || (choice[0] == '2')) && (strlen(choice) == 1)){
            break;
        }else{
            printf("Неверно введён пункт меню. Попробуйте ещё раз.\n");
            free(choice);
            choice = readline("");
        }
    }
    return choice;
}

Table* make_table(int size, char* fname){
    Table* ptable = (Table*)calloc(1, sizeof(Table));
    ptable->ks_f2 = (KeySpace2_f*)calloc(size, sizeof(KeySpace2_f));
    for(int i = 0; i < size; i++){
        ptable->ks_f2[i].elem_of = -1;
    }
    ptable->csize2 = 0;
    ptable->msize2 = size;
    ptable->fname = (char*)calloc(strlen(fname) + 1, sizeof(char));
    strcpy(ptable->fname, fname);
    ptable->file = fopen(fname, "w+b");
    fseek(ptable->file, 0, SEEK_SET);
    fwrite(&(ptable->msize2), sizeof(int), 1, ptable->file);
    for(int i = 0; i < size; i++){
        fwrite(&(ptable->ks_f2[i].elem_of), sizeof(int), 1, ptable->file);
    }
    fclose(ptable->file);
    free(fname);
    return ptable;
}

Table* get_from_file(char* fname){
    Table* ptable = (Table*)calloc(1, sizeof(Table));
    ptable->fname = (char*)calloc(strlen(fname) + 1, sizeof(char));
    strcpy(ptable->fname, fname);
    ptable->file = fopen(fname, "r+b");
    ptable->csize2 = 0;
    if(ptable->file == NULL){
        free(ptable->fname);
        free(ptable);
        free(fname);
        ptable = NULL;
        return ptable;
    }
    fseek(ptable->file, 0, SEEK_SET);
    ptable->msize2 = 0;
    int n = 0;
    fread(&n, sizeof(int), 1, ptable->file);
    ptable->msize2 = n;
    if(n == 0){
        free(ptable);
        free(fname);
        ptable = NULL;
        return ptable;
    }
    ptable->ks_f2 = (KeySpace2_f*)calloc(ptable->msize2, sizeof(KeySpace2_f));
    for(int i = 0; i < ptable->msize2; i++){
        fread(&(ptable->ks_f2[i].elem_of), sizeof(int), 1, ptable->file);
        if(ptable->ks_f2[i].elem_of != -1){
            ptable->csize2 = ptable->csize2 + 1;
        }
    }
    free(fname);
    return ptable;
}


int insert(char* key, char* inf, Table* ptable){
    int strt = 1;
    int i = 1;
    int key_l = 0;
    int inf_l = 0;
    if(ptable->msize2 == ptable->csize2){
        return -2;//переполнение
    }
    int j = 1;
    ptable->file = fopen(ptable->fname, "r+b");
    while((ptable->ks_f2[i].elem_of != -1) && (j < ptable->msize2)){
        fseek(ptable->file, ptable->ks_f2[i].elem_of, SEEK_SET);
        fread(&key_l, sizeof(int), 1, ptable->file);
        char* key_f = (char*)calloc(key_l + 1, sizeof(char));
        fread(key_f, sizeof(char), key_l, ptable->file);
        if(strcmp(key_f, key) == 0){
        }
        i++;
        free(key_f);
        key_f = NULL;
        key_l = 0;
        j++;
    }
    fclose(ptable->file);
    ptable->file = fopen(ptable->fname, "r+b");
    fseek(ptable->file, 0, SEEK_END);
    ptable->ks_f2[i].elem_of = ftell(ptable->file);
    fseek(ptable->file, 4 + i*(sizeof(int)), SEEK_SET);
    fwrite(&(ptable->ks_f2[i].elem_of), sizeof(int), 1, ptable->file);
    fseek(ptable->file, 0, SEEK_END);
    key_l = strlen(key);
    inf_l = strlen(inf);
    fwrite(&key_l, sizeof(int), 1, ptable->file);
    fwrite(key, sizeof(char), key_l, ptable->file);
    fwrite(&inf_l, sizeof(int), 1, ptable->file);
    fwrite(inf, sizeof(char), inf_l, ptable->file);
    fclose(ptable->file);
    ptable->csize2 = ptable->csize2 + 1;
    return 0;
}

Table_old* search(Table* ptable, char* key){
    Table_old* min_table = (Table_old*)calloc(1, sizeof(Table_old));
    min_table->ks = (KeySpace2*)calloc(ptable->msize2, sizeof(KeySpace2));
    int i = 0;
    int num = 0;
    int key_l = 0;
    int inf_l = 0;
    min_table->msize2 = ptable->msize2;
    ptable->file = fopen(ptable->fname, "r+b");
    while(num < ptable->csize2){
        if(ptable->ks_f2[i].elem_of != -1){
            fseek(ptable->file,ptable->ks_f2[i].elem_of, SEEK_SET);
            fread(&key_l, sizeof(int), 1, ptable->file);
            char* key_f = (char*)calloc(key_l + 1, sizeof(char));
            fread(key_f, sizeof(char), key_l, ptable->file);
            if(strcmp(key_f, key) == 0){
                min_table->ks[i].busy = 1;
                fread(&key_l, sizeof(int), 1, ptable->file);
                min_table->ks[i].key = (char*)calloc(key_l + 1, sizeof(char));
                strcpy(min_table->ks[i].key, key_f);
                min_table->csize2 = min_table->csize2 + 1;
                fread(&inf_l, sizeof(int), 1, ptable->file);
                min_table->ks[i].info = (char*)calloc(inf_l + 1, sizeof(char));
                fread(min_table->ks[i].info, sizeof(char), inf_l, ptable->file);
                
            }
            num++;
            free(key_f);
        }
        i++;
        key_l = 0;
        inf_l = 0;
    }
    fclose(ptable->file);
    return min_table;
}

int delete_k(Table* ptable, char* key){
    int i = 0;
    int num = 0;
    int f = 0;
    int k = 0;
    int key_l = 0;
    ptable->file = fopen(ptable->fname, "r+b");
    while(num < ptable->csize2){
        if(ptable->ks_f2[i].elem_of != -1){
            fseek(ptable->file, ptable->ks_f2[i].elem_of, SEEK_SET);
            fread(&key_l, sizeof(int), 1, ptable->file);
            char* key_f = (char*)calloc(key_l + 1, sizeof(char));
            fread(key_f, sizeof(char), key_l, ptable->file);
            if(strcmp(key_f, key) == 0){
                ptable->ks_f2[i].elem_of = -1;
                fseek(ptable->file, 4 + i*(sizeof(int)), SEEK_SET);
                fwrite(&(ptable->ks_f2[i].elem_of), sizeof(int), 1, ptable->file);
                k++;
                f = 1;
            }
            free(key_f);
            num++;
        }
        i++;
        key_l = 0;
    }
    ptable->csize2 = ptable->csize2 - k;
    fclose(ptable->file);
    if(f){
        return 0;
    }else{
        return -1;
    }
}

Table_old* indiv_search(Table* ptable, char* key){
    Table_old* min_table = (Table_old*)calloc(1, sizeof(Table_old));
    min_table->ks = (KeySpace2*)calloc(1, sizeof(KeySpace2));
    int i = 0;
    int num = 0;
    int key_l = 0;
    int inf_l = 0;
    ptable->file = fopen(ptable->fname, "r+b");
    while(num < ptable->csize2){
        if(ptable->ks_f2[i].elem_of != -1){
            fseek(ptable->file, ptable->ks_f2[i].elem_of, SEEK_SET);
            fread(&key_l, sizeof(int), 1, ptable->file);
            char* key_f = (char*)calloc(key_l + 1, sizeof(char));
            fread(key_f, sizeof(char), key_l, ptable->file);
            if((strcmp(key_f, key) == 0)){
                min_table->ks[0].busy = 1;
                fread(&inf_l, sizeof(int), 1, ptable->file);
                min_table->ks[0].info = (char*)calloc(inf_l + 1, sizeof(char));
                fread(min_table->ks[0].info, sizeof(char), inf_l, ptable->file);
                min_table->ks[0].key = (char*)calloc(key_l + 1, sizeof(char));
                strcpy(min_table->ks[0].key, key_f);
                min_table->csize2 = 1;
            }
            free(key_f);
            key_f = NULL;
            num++;
            key_l = 0;
            inf_l = 0;
        }
        i++;
    }
    fclose(ptable->file);
    return min_table;
}

void print_table(Table_old* ptable){
    printf("   i    |   key   |   info   \n");
    int i = 0;
    int num = 0;
    while(num < ptable->csize2){
        if(ptable->ks[i].busy > 0){
            printf(" %d) | %s | %s ", i, ptable->ks[i].key, ptable->ks[i].info);
            printf("\n");
            num++;
        }
        i++;
    }
}

Table_old* free_table(Table_old* ptable){
    int i = 0;
    int num = 0;
    while(num < ptable->csize2){
        if(ptable->ks[i].busy > 0){
            free(ptable->ks[i].info);
            free(ptable->ks[i].key);
            num++;
        }
        i++;
    }
    free(ptable->ks);
    free(ptable);
    ptable = NULL;
    return ptable;
}

void print_from_file(Table* ptable){
    printf("   i    |   key   |   info   \n");
    int i = 0;
    int num = 0;
    int key_l = 0;
    int inf_l = 0;
    ptable->file = fopen(ptable->fname, "r+b");
    while(num < ptable->csize2){
        if(ptable->ks_f2[i].elem_of != -1){
            fseek(ptable->file, ptable->ks_f2[i].elem_of, SEEK_SET);
            fread(&key_l, sizeof(int), 1, ptable->file);
            char* key = (char*)calloc(key_l + 1, sizeof(char));
            fread(key, sizeof(char), key_l, ptable->file);
            fread(&inf_l, sizeof(int), 1, ptable->file);
            char* info = (char*)calloc(inf_l + 1, sizeof(char));
            fread(info, sizeof(char), inf_l, ptable->file);
            printf(" %d) | %s | %s ", i, key, info);
            printf("\n");
            num++;
            free(key);
            key = NULL;
            free(info);
            info = NULL;
            key_l = 0;
            inf_l = 0;
        }
        i++;
    }
    fclose(ptable->file);
}

Table* free_ftable(Table* ptable){
    int i = 0;
    int num = 0;
    while(num < ptable->csize2){
        if(ptable->ks_f2[i].elem_of != -1){
            ptable->ks_f2[i].elem_of = -1;
            num++;
        }
        i++;
    }
    free(ptable->ks_f2);
    free(ptable->fname);
    free(ptable);
    ptable = NULL;
    return ptable;
}