#include "headings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int compare_type_name(type a, type b, int dir){
    return strcmp(a.name, b.name)*dir;
}

int compare_type_height(type a, type b, int dir){
    if (a.high > b.high) return 1 * dir;
    if (a.high < b.high) return -1 * dir;
    return 0;
}

int compare_type_price(type a, type b, int dir){
    if (a.price_item > b.price_item) return 1 * dir;
    if (a.price_item < b.price_item) return -1 * dir;
    return 0;
}

int compare_pair_dominant(pair a, pair b, int dir){
    if (a.code_dominant > b.code_dominant) return 1 * dir;
    if (a.code_dominant < b.code_dominant) return -1 * dir;
    return 0;
}

int compare_pair_sumcost(pair a, pair b, int dir){
    if (a.sum_cost > b.sum_cost) return 1 * dir;
    if (a.sum_cost < b.sum_cost) return -1 * dir;
    return 0;
}

void sorting_process_type(type* array,  int left, int right, int (*comparator)(type, type, int ), int dir){
    type center = array[(right+left) / 2];
    int l = left;
    int r = right;
    do{
        while (comparator(array[l], center, dir)<0){ //array[l] < center
            l++;
        }
        while (comparator(array[r], center, dir)>0){
            r--;
        }
        if (l<=r){
            type temp = array[l];
            array[l] = array[r];
            array[r] = temp;
            l++;
            r--;
        }
    } while (l<=r);
    if (r > left){
        sorting_process_type(array, left, r, comparator, dir);
    }
    if (l < right){
        sorting_process_type(array, l, right, comparator, dir);
    }
}


void sorting_process_pair(pair* array,  int left, int right, int (*comparator)(pair, pair, int), int dir){
    pair center = array[(right+left) / 2];
    int l = left;
    int r = right;
    do{
        while (comparator(array[l], center, dir)<0){ //array[l] < center
            l++;
        }
        while (comparator(array[r], center, dir)>0){
            r--;
        }
        if (l<=r){
            pair temp = array[l];
            array[l] = array[r];
            array[r] = temp;
            l++;
            r--;
        }
    } while (l<=r);

    if (r > left){
        sorting_process_pair(array, left, r, comparator, dir);
    }
    if (l < right){
        sorting_process_pair(array, l, right, comparator, dir);
    }
}

type* f_copy_types (info_types* inf_types, int* count_new){
    type* copy_types = malloc(inf_types->count_types * sizeof(type));
    if (copy_types == NULL) {
        printf("Ошибка выделения памяти для копии для массива типов\n");
        return NULL;
    }
    int actual_count = 0;
    for (int i = 0; i < inf_types->count_types; i++){
        if (inf_types->types[i].is_deleted == 0){
            // Копируем структуру
            copy_types[actual_count] = inf_types->types[i];
            
            // ГЛУБОКОЕ копирование названия
            copy_types[actual_count].name = strdup(inf_types->types[i].name);
            if (copy_types[actual_count].name == NULL){
                printf("Ошибка выделения памяти для копии названия\n");
                for (int j = 0; j < actual_count; j++){
                    free(copy_types[j].name);
                }
                free(copy_types);
                return NULL;
            }
            
            // ГЛУБОКОЕ копирование массива id_pair_domin
            if (inf_types->types[i].count_pairs > 0){
                copy_types[actual_count].id_pair_domin = malloc(inf_types->types[i].capacity_pairs * sizeof(int));
                if (copy_types[actual_count].id_pair_domin == NULL){
                    printf("Ошибка выделения памяти для копии массива id пар\n");
                    free(copy_types[actual_count].name);
                    for (int j = 0; j < actual_count; j++){
                        free(copy_types[j].name);
                        if (copy_types[j].id_pair_domin) free(copy_types[j].id_pair_domin);
                    }
                    free(copy_types);
                    return NULL;
                }
                memcpy(copy_types[actual_count].id_pair_domin, // 1. КУДА копируем
                        inf_types->types[i].id_pair_domin, // 2. ОТКУДА копируем
                        inf_types->types[i].count_pairs * sizeof(int));   // 3. СКОЛЬКО БАЙ
            } else {
                copy_types[actual_count].id_pair_domin = NULL;
            }
            actual_count++;
        }
    }
    *count_new = actual_count;
    return copy_types;
}

pair* f_copy_pairs (info_pairs* inf_pairs, int* count_new){
    pair* copy_pairs = malloc(inf_pairs->count_pairs*sizeof(pair));
    if (copy_pairs == NULL) {
        printf("Ошибка выделения памяти для копии массива совместимостей\n");
        return NULL;
    }
    int actual_count = 0;
    for (int i=0; i<inf_pairs->count_pairs; i++){
        if (inf_pairs->pairs[i].is_deleted == 0){
            copy_pairs[actual_count] = inf_pairs->pairs[i];
            actual_count++;
        } 
    }
    *count_new = actual_count;
    return copy_pairs;
}

int sort_types(info_types* inf_types, int (*comparator) (type, type, int), int dir){
    if (inf_types->count_types < 2) {
        view_types(inf_types);
        return 0;
    }
    int count_new = 0;
    type* copy_types = f_copy_types(inf_types, &count_new);
    if (copy_types == NULL){
        return 1;
    }
    if (count_new == 0) {
        printf("Нет активных записей для сортировки\n");
        free(copy_types);
        return 0;
    }
    int left = 0;
    int right = count_new - 1;
    if (!copy_types) return 1;
    sorting_process_type(copy_types, left, right, comparator, dir);
    info_types copy_inf_types = {copy_types, count_new, count_new};
    view_types(&copy_inf_types);
    for (int i = 0; i < count_new; i++) {
        // память из-под скопированного имени
        free(copy_types[i].name); 
        //  память из-под массива ID доминирующих пар (если он был выделен)
        if (copy_types[i].id_pair_domin != NULL) {
            free(copy_types[i].id_pair_domin);
        }
    }
    free(copy_types);
    return 0;
}

int sort_pairs(info_pairs* inf_pairs, int (*comparator) (pair, pair, int), int dir){
    if (inf_pairs->count_pairs < 2) {
        view_pairs(inf_pairs);
        return 0;
    }
    int count_new = 0;
    pair* copy_pairs = f_copy_pairs (inf_pairs, &count_new);
    
    if (copy_pairs == NULL) {
        return 1; 
    }
    
    if (count_new == 0) {
        printf("Нет активных записей для сортировки\n");
        free(copy_pairs);
        return 0;
    }
    int left = 0;
    int right = count_new - 1;
    sorting_process_pair(copy_pairs, left, right, comparator, dir);
    info_pairs copy_inf_pairs = {copy_pairs, count_new, count_new};
    view_pairs(&copy_inf_pairs);
    free(copy_pairs);
    return 0;
}

