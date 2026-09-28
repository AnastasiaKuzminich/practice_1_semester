#include "headings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void clear_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int choose_spisok(){
    printf("==============================\n");
    printf("Выберите список:\n");
    printf("1. Список типов цветков\n");
    printf("2. Список совместимостей\n");
    printf("==============================\n");
    int choice_view;
    while (1){
        if (scanf("%d", &choice_view) != 1){
            printf("Введите цифру для выбора списка.\n");
            clear_buffer();
            continue;
        }
        clear_buffer();
        if (choice_view < 1 || choice_view > 2){
            printf("Введите 1 или 2 для выбора списка.\n");
            continue;
        }
        break;
    }
    return choice_view;
}

void free_types(info_types* inf_types){
    if (inf_types == NULL || inf_types->types == NULL) return;
    for (int i = 0; i<inf_types->count_types; i++){
        free(inf_types->types[i].name);
        if (inf_types->types[i].id_pair_domin != NULL) {
            free(inf_types->types[i].id_pair_domin);
        }
    }
    free(inf_types->types);
    inf_types->types = NULL;
    inf_types->count_types = 0;
    inf_types->capacity_types = 0;
}

void free_pairs(info_pairs* inf_pairs){
    if (inf_pairs == NULL || inf_pairs->pairs == NULL) return;
    
    free(inf_pairs->pairs);
    inf_pairs->pairs = NULL;
    inf_pairs->count_pairs = 0;
    inf_pairs->capacity_pairs = 0;
}