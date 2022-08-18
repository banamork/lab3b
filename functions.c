#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <readline/readline.h>
#include <ctype.h>
#include "functions.h"
void showMenu(){
    printf("\n ..................................................................... \n");
    printf("Выберите действие: \n");
    printf("0 - Выйти из программы.\n");
    printf("1 - Вставить элемент в таблицу.\n");
    printf("2 - Поиск в таблице элемента по ключу.\n");
    printf("3 - Удаление из таблицы элемента по ключу.\n");
    printf("4 - Вывод содержимого таблицы.\n");
    printf("\n ..................................................................... \n");
}

int verify(char* menu){
    while(1){
        if(menu == NULL){
            printf("Вы ничего не ввели.\n");
            menu = readline("");
        }else if(strlen(menu) == 0){
            printf("Вы ничего не ввели.\n");
            free(menu);
            menu = readline("");
        }else if(strlen(menu) == 1){
            if(isdigit(menu[0])){
                int k = atoi(menu);
                free(menu);
                return k;
            }else{
                printf("Неверно введён пункт меню.\n");
                free(menu);
                menu = readline("");
            }
        }else{
            printf("Неверно введён пункт меню.\n");
            free(menu);
            menu = readline("");
        }
    }
}

int if_stupid(char* size){
    while(1){
        if(size == NULL){
            printf("Вы ничего не ввели.\n");
            size = readline("");
        }else if(strlen(size) == 0){
            printf("Вы ничего не ввели.\n");
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
                printf("Неверно введён размер.\n");
                free(size);
                size = readline("");
            }
        }
    }
}

char* verify_name_of_file(char* filename){
    while(1){
        if(filename == NULL){
            printf("Вы ничего не ввели.\n");
            filename = readline("");
        }else if(strlen(filename) == 0){
            free(filename);
            printf("Вы ничего не ввели.\n");
            filename = readline("");
        }else{
            break;
        }
    }
    return filename;
}

char* verify_selected(char* selected){
    while(1){
        if(selected == NULL){
            printf("Вы ничего не ввели.\n");
            selected = readline("");
        }else if(strlen(selected) == 0){
            printf("Вы ничего не ввели.\n");
            free(selected);
            selected = readline("");
        }else if(((selected[0] == '1') || (selected[0] == '2')) && (strlen(selected) == 1)){
            break;
        }else{
            printf("Неверно введён пункт меню.\n");
            free(selected);
            selected = readline("");
        }
    }
    return selected;
}

Table* create_table(int size, char* filename){
    Table* newtable = (Table*)calloc(1, sizeof(Table));
    newtable->ks_f = (KeySpace2_f*)calloc(size, sizeof(KeySpace2_f));
    for(int i = 0; i < size; i++){
        newtable->ks_f[i].element_f = -1;
    }
    newtable->size2b = 0;
    newtable->size2a = size;
    newtable->filename = (char*)calloc(strlen(filename) + 1, sizeof(char));
    strcpy(newtable->filename, filename);
    newtable->file = fopen(filename, "w+b");
    fseek(newtable->file, 0, SEEK_SET);
    fwrite(&(newtable->size2a), sizeof(int), 1, newtable->file);
    for(int i = 0; i < size; i++){
        fwrite(&(newtable->ks_f[i].element_f), sizeof(int), 1, newtable->file);
    }
    fclose(newtable->file);
    free(filename);
    return newtable;
}

Table* bookmark_to_file(char* filename){
    Table* newtable = (Table*)calloc(1, sizeof(Table));
    newtable->filename = (char*)calloc(strlen(filename) + 1, sizeof(char));
    strcpy(newtable->filename, filename);
    newtable->file = fopen(filename, "r+b");
    newtable->size2b = 0;
    if(newtable->file == NULL){
        free(newtable->filename);
        free(newtable);
        free(filename);
        newtable = NULL;
        return newtable;
    }
    fseek(newtable->file, 0, SEEK_SET);
    newtable->size2a = 0;
    int n = 0;
    fread(&n, sizeof(int), 1, newtable->file);
    newtable->size2a = n;
    if(n == 0){
        free(newtable);
        free(filename);
        newtable = NULL;
        return newtable;
    }
    newtable->ks_f = (KeySpace2_f*)calloc(newtable->size2a, sizeof(KeySpace2_f));
    for(int i = 0; i < newtable->size2a; i++){
        fread(&(newtable->ks_f[i].element_f), sizeof(int), 1, newtable->file);
        if(newtable->ks_f[i].element_f != -1){
            newtable->size2b = newtable->size2b + 1;
        }
    }
    free(filename);
    return newtable;
}


int push(char* key, char* info, Table* newtable){
    int strt = 1;
    int i = 1;
    int key_l = 0;
    int info_l = 0;
    if(newtable->size2a == newtable->size2b){   //underflow
        return -2;
    }
    int j = 1;
    newtable->file = fopen(newtable->filename, "r+b");
    while((newtable->ks_f[i].element_f != -1) && (j < newtable->size2a)){
        fseek(newtable->file, newtable->ks_f[i].element_f, SEEK_SET);
        fread(&key_l, sizeof(int), 1, newtable->file);
        char* key_f = (char*)calloc(key_l + 1, sizeof(char));
        fread(key_f, sizeof(char), key_l, newtable->file);
        if(strcmp(key_f, key) == 0){
            printf("Ключ уже существует.\n")
            ;return -1;
        }
        i++;
        free(key_f);
        key_f = NULL;
        key_l = 0;
        j++;
    }
    fclose(newtable->file);
    newtable->file = fopen(newtable->filename, "r+b");
    fseek(newtable->file, 0, SEEK_END);
    newtable->ks_f[i].element_f = ftell(newtable->file);
    fseek(newtable->file, 4 + i*(sizeof(int)), SEEK_SET);
    fwrite(&(newtable->ks_f[i].element_f), sizeof(int), 1, newtable->file);
    fseek(newtable->file, 0, SEEK_END);
    key_l = strlen(key);
    info_l = strlen(info);
    fwrite(&key_l, sizeof(int), 1, newtable->file);
    fwrite(key, sizeof(char), key_l, newtable->file);
    fwrite(&info_l, sizeof(int), 1, newtable->file);
    fwrite(info, sizeof(char), info_l, newtable->file);
    fclose(newtable->file);
    newtable->size2b = newtable->size2b + 1;
    return 0;
}

old_Table* find(Table* newtable, char* key){
    old_Table* min_table = (old_Table*)calloc(1, sizeof(old_Table));
    min_table->ks_l = (KeySpace2_l*)calloc(newtable->size2a, sizeof(KeySpace2_l));
    int i = 0;
    int num = 0;
    int key_l = 0;
    int info_l = 0;
    min_table->old_size2a = newtable->size2a;
    newtable->file = fopen(newtable->filename, "r+b");
    while(num < newtable->size2b){
        if(newtable->ks_f[i].element_f != -1){
            fseek(newtable->file,newtable->ks_f[i].element_f, SEEK_SET);
            fread(&key_l, sizeof(int), 1, newtable->file);
            char* key_f = (char*)calloc(key_l + 1, sizeof(char));
            fread(key_f, sizeof(char), key_l, newtable->file);
            fread(&info_l, sizeof(int), 1, newtable->file);
            char* info_f = (char*)calloc(info_l + 1, sizeof(char));
            fread(info_f, sizeof(char), info_l, newtable->file);
            if(strcmp(key_f, key) == 0){
                min_table->ks_l[i].busy = 1;
                fread(&key_l, sizeof(int), 1, newtable->file);
                min_table->ks_l[i].key = (char*)calloc(key_l + 1, sizeof(char));
                strcpy(min_table->ks_l[i].key, key_f);
                min_table->old_size2b = min_table->old_size2b + 1;
                fread(&info_l, sizeof(int), 1, newtable->file);
                min_table->ks_l[i].info = (char*)calloc(info_l + 1, sizeof(char));
                fread(min_table->ks_l[i].info, sizeof(char), info_l, newtable->file);
                strcpy(min_table->ks_l[i].info, info_f);
            }
            num++;
            free(key_f);
            free(info_f);
        }
        i++;
        key_l = 0;
        info_l = 0;
    }
    fclose(newtable->file);
    return min_table;
}


int remove_key(Table* newtable, char* key){
    int i = 0;
    int num = 0;
    int d = 0;
    int j = 0;
    int key_l = 0;
    newtable->file = fopen(newtable->filename, "r+b");
    while(num < newtable->size2b){
        if(newtable->ks_f[i].element_f != -1){
            fseek(newtable->file, newtable->ks_f[i].element_f, SEEK_SET);
            fread(&key_l, sizeof(int), 1, newtable->file);
            char* key_f = (char*)calloc(key_l + 1, sizeof(char));
            fread(key_f, sizeof(char), key_l, newtable->file);
            if(strcmp(key_f, key) == 0){
                newtable->ks_f[i].element_f = -1;
                fseek(newtable->file, 4 + i*(sizeof(int)), SEEK_SET);
                fwrite(&(newtable->ks_f[i].element_f), sizeof(int), 1, newtable->file);
                j++;
                d = 1;
            }
            free(key_f);
            num++;
        }
        i++;
        key_l = 0;
    }
    newtable->size2b = newtable->size2b - j;
    fclose(newtable->file);
    if(d){
        return 0;
    }else{
        return -1;
    }
}


void show_oldtable(old_Table* newtable){
    printf("   i    |   key   |   info   \n");
    int i = 0;
    int num = 0;
    while(num < newtable->old_size2b){
        if(newtable->ks_l[i].busy > 0){
            printf(" %d) | %s | %s ", i, newtable->ks_l[i].key, newtable->ks_l[i].info);
            printf("\n");
            num++;
        }
        i++;
    }
}

old_Table* clean_oldtable(old_Table* newtable){
    int i = 0;
    int num = 0;
    while(num < newtable->old_size2b){
        if(newtable->ks_l[i].busy > 0){
            free(newtable->ks_l[i].info);
            free(newtable->ks_l[i].key);
            num++;
        }
        i++;
    }
    free(newtable->ks_l);
    free(newtable);
    newtable = NULL;
    return newtable;
}

void viewer_to_file(Table* newtable){
    printf("   i    |   key   |   info   \n");
    int i = 0;
    int num = 0;
    int key_l = 0;
    int info_l = 0;
    newtable->file = fopen(newtable->filename, "r+b");
    while(num < newtable->size2b){
        if(newtable->ks_f[i].element_f != -1){
            fseek(newtable->file, newtable->ks_f[i].element_f, SEEK_SET);
            fread(&key_l, sizeof(int), 1, newtable->file);
            char* key = (char*)calloc(key_l + 1, sizeof(char));
            fread(key, sizeof(char), key_l, newtable->file);
            fread(&info_l, sizeof(int), 1, newtable->file);
            char* info = (char*)calloc(info_l + 1, sizeof(char));
            fread(info, sizeof(char), info_l, newtable->file);
            printf(" %d) | %s | %s ", i, key, info);
            printf("\n");
            num++;
            free(key);
            key = NULL;
            free(info);
            info = NULL;
            key_l = 0;
            info_l = 0;
        }
        i++;
    }
    fclose(newtable->file);
}

Table* clean_table(Table* newtable){
    int i = 0;
    int num = 0;
    while(num < newtable->size2b){
        if(newtable->ks_f[i].element_f != -1){
            newtable->ks_f[i].element_f = -1;
            num++;
        }
        i++;
    }
    free(newtable->ks_f);
    free(newtable->filename);
    free(newtable);
    newtable = NULL;
    return newtable;
}