#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "functions.h"
int main(){
        int n = 1;
    Table* newtable = NULL;
    char* filename = NULL;
    printf("1 - Новый файл.\n");
    printf("2 - Ввести таблицу из существующего.\n");
    char* selected = readline("");
    selected = verify_selected(selected);
    printf("Имя файла: \n");
    filename = readline("");
    filename = verify_name_of_file(filename);
    if(selected[0] == '1'){
        printf("Размер таблицы: \n");
        char* s = readline("");
        int size = if_stupid(s);
        newtable = create_table(size, filename);
    }else{
        newtable = bookmark_to_file(filename);
    }
    free(selected);

    while(n){
        showMenu();
        char* menu = readline("");
        int k = verify(menu);
        switch(k){
            case 0:
                n = 0;
                if(newtable != NULL){
                    newtable = clean_table(newtable);
                }
                break;
            case 1:
                if(newtable == NULL){
                    printf("1 - Новый файл.\n");
                    printf("2 - Ввести таблицу из существующего.\n");
                    char* selected = readline("");
                    selected = verify_selected(selected);
                    printf("Имя файла: \n");
                    filename = readline("");
                    filename = verify_name_of_file(filename);
                    if(selected[0] == '1'){
                        printf("Размер тaблицы: \n");
                        char* s = readline("");
                        int size = if_stupid(s);
                        newtable = create_table(size, filename);
                    }else{
                        newtable = bookmark_to_file(filename);
                    }
                    free(selected);
                }
                printf("Введите ключ: \n");
                char* key = readline("");
                printf("Введите информацию: \n");
                char* info = readline("");
                if(newtable == NULL){
                    printf("Таблица не создана.\n");
                }else{
                    int d = push(key, info, newtable);
                    if(d == -2){
                        printf("Таблица переполнена.\n");
                    }
                }
                free(info);
                free(key);
                break;
            case 2:
                if(newtable == NULL){
                    printf("Вы не ввели таблицу.\n");
                    break;
                }
                printf("Введите ключ для поиска: \n");
                key = readline("");
                old_Table* min_table = find(newtable, key);
                if(min_table->old_size2b > 0){
                    show_oldtable(min_table);
                }else{
                    printf("Элемент не найден.\n");
                }
                free(key);
                min_table = clean_oldtable(min_table);
                break;
            case 3:
                if(newtable == NULL){
                    printf("Вы не ввели таблицу.\n");
                    break;
                }
                printf("Введите ключ для удаления: \n");
                key = readline("");
                int d = remove_key(newtable, key);
                if(d){
                    printf("Элемент не найден.\n");
                }
                free(key);
                break;
            case 4:
                if(newtable == NULL){
                    printf("Вы не ввели таблицу.\n");
                    break;
                }else{
                    viewer_to_file(newtable);
                }
                break;
            default:
                break;

        }
    }
    return 0;
}