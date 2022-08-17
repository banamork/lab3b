#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "functions.h"
int main(){
        int w = 1;
    Table* ptable = NULL;
    char* fname = NULL;
    printf("1 - Создать новый файл\n");
    printf("2 - Ввести таблицу из уже существующего\n");
    char* choice = readline("");
    choice = check_choice(choice);
    printf("Введите имя файла: \n");
    fname = readline("");
    fname = check_file_name(fname);
    if(choice[0] == '1'){
        printf("Введите размер таблицы: \n");
        char* s = readline("");
        int size = check_if_you_an_idiot(s);
        ptable = make_table(size, fname);
    }else{
        ptable = get_from_file(fname);
    }
    free(choice);

    while(w){
        printMenu();
        char* menu = readline("");
        int m = check(menu);
        switch(m){
            case 0:
                w = 0;
                if(ptable != NULL){
                    ptable = free_ftable(ptable);
                }
                break;
            case 1:
                if(ptable == NULL){
                    printf("1 - Создать новый файл\n");
                    printf("2 - Ввести таблицу из уже существующего\n");
                    char* choice = readline("");
                    choice = check_choice(choice);
                    printf("Введите имя файла: \n");
                    fname = readline("");
                    fname = check_file_name(fname);
                    if(choice[0] == '1'){
                        printf("Введите размер тaблицы: \n");
                        char* s = readline("");
                        int size = check_if_you_an_idiot(s);
                        ptable = make_table(size, fname);
                    }else{
                        ptable = get_from_file(fname);
                    }
                    free(choice);
                }
                printf("Введите ключ: \n");
                char* key = readline("");
                printf("Введите информацию: \n");
                char* inf = readline("");
                if(ptable == NULL){
                    printf("Таблица не создана.\n");
                }else{
                    int f = insert(key, inf, ptable);
                    if(f == -2){
                        printf("Таблица переполнена.\n");
                    }
                }
                free(inf);
                free(key);
                break;
            case 2:
                if(ptable == NULL){
                    printf("Вы не ввели таблицу. Введите таблицу.\n");
                    break;
                }
                printf("Введите ключ для поиска\n");
                key = readline("");
                Table_old* min_table = search(ptable, key);
                if(min_table->csize2 > 0){
                    print_table(min_table);
                }else{
                    printf("Элемент не найден.\n");
                }
                free(key);
                min_table = free_table(min_table);
                break;
            case 3:
                if(ptable == NULL){
                    printf("Вы не ввели таблицу. Введите таблицу.\n");
                    break;
                }
                printf("Введите ключ для удаления\n");
                key = readline("");
                int f = delete_k(ptable, key);
                if(f){
                    printf("Элемент в таблице не найден\n");
                }
                free(key);
                break;
            case 4:
                if(ptable == NULL){
                    printf("Введите таблицу.\n");
                    break;
                }else{
                    print_from_file(ptable);
                }
                break;
            default:
                break;

        }
    }
    return 0;
}