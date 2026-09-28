#include "headings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int comparator_code_filtr(type a, void* b){
    int target_code = *(int*)b;
    return a.code_type - target_code;
}

int comparator_name_filtr(type a, void* b){
    char* target_name = (char*)b;
    return strcmp(a.name, target_name);
}

int comparator_price_filtr(type a, void* b){
    double target_price = *(double*)b;
    double eps = 0.0001;
    if(a.price_item > target_price + eps)  return 1;
    if(a.price_item < target_price - eps)  return -1;
    return 0;
}

int comparator_id_pair_filtr(pair a, void* b){
    int target_code = *(int*)b;
    return a.id_pair - target_code;
}

int comparator_code_dominant_filtr(pair a, void* b){
    int target_code = *(int*)b;
    return a.code_dominant - target_code;
}

int comparator_sumcost_filtr(pair a, void* b){
    double target_sumcost = *(double*)b;
    double eps = 0.0001;
    if(a.sum_cost > target_sumcost + eps)  return 1;
    if(a.sum_cost < target_sumcost - eps)  return -1;
    return 0;
}

int bin_search_type(int L, int R, type* types, void* kriterij, int (*comparator)(type a, void* b)){
    if (L > R) return -1;
    int mid = L + (R - L) / 2;
    int cmp = comparator(types[mid], kriterij);
    if (cmp == 0) return mid;
    if (cmp > 0) {
        return bin_search_type(L, mid - 1, types, kriterij, comparator);
    }else{
    return bin_search_type(mid + 1, R, types, kriterij, comparator);
    }
}

// возвращает индекс найденного элемента в текущем массиве
int bin_search_pair(int L, int R, pair* pairs, void* kriterij, int (*comparator)(pair a, void* b)){
    if (pairs == NULL || L > R) {
        return -1; 
    }
    int mid = L + (R - L) / 2;
    int cmp = comparator(pairs[mid], kriterij);
    if (cmp == 0) return mid;
    if (cmp > 0) {
        return bin_search_pair(L, mid - 1, pairs, kriterij, comparator);
    }else{
    return bin_search_pair(mid + 1, R, pairs, kriterij, comparator);
    }
}

void filter_type_by_code(info_types inf_types){
    printf("--------------------\n");
    printf("Введите КОД типа цветка для осуществления поиска по нему\n");
    int filtr_code;
    while (1){
        if (scanf("%d", &filtr_code) != 1 || filtr_code < 0){
            printf("Введите положительное число");
            clear_buffer();
            continue;
        }
        clear_buffer();
        break;
    }
    printf("--------------------\n");
    // поиск одного элемента по уникальному ключу 
    if (inf_types.count_types == 0) {
        printf("Нет активных записей для поиска по коду типа\n");
        return;
    }
    int index = bin_search_type(0, inf_types.count_types-1, inf_types.types, &filtr_code, comparator_code_filtr);
    if (index == -1 || inf_types.types[index].is_deleted == 1){
        printf("Элемент списка типов с кодом %d не найден\n", filtr_code);
        return;
    }
    printf("===ИНФОРМАЦИЯ О НАЙДЕННОМ ЭЛЕМЕНТЕ===\n");
    view_one_type (inf_types.types[index]);
    return;
}

void filter_type_by_name(info_types inf_types){
    printf("--------------------\n");
    char filtr_name[LEN_NAME];
    while (1){
        printf("Введите НАЗВАНИЕ типа цветка для осуществления поиска по нему\n");
        if (fgets(filtr_name, LEN_NAME, stdin) == NULL){
            printf("Ошибка ввода названия типа цветка\n");
            continue;
        }
        filtr_name[strcspn(filtr_name, "\n")] = '\0';
        
        break;
    }    
    printf("--------------------\n");
    // поиск всех элементов по имени (имена могут повторяться)
    // сначала создание копии массива, сортировка, далее бинарный поиск одного, расширение для поиска начала и конца, вывод всех из отрезка
    int count_new;
    type* copy_types_filtr_name = f_copy_types (&inf_types, &count_new);
    
    if (copy_types_filtr_name == NULL){
        printf("Ошибка создания копии массива при осуществлении бинарного поиска\n");
        return;
    }
    if (count_new == 0) {
        printf("Нет активных записей для поиска по названю типа\n");
        free(copy_types_filtr_name);
        return;
    }
    info_types copy_inf_types = {copy_types_filtr_name, count_new, count_new};
    sorting_process_type(copy_types_filtr_name, 0, count_new-1, compare_type_name, 1);
    int index = bin_search_type(0, count_new-1, copy_types_filtr_name, filtr_name, comparator_name_filtr);
    if (index == -1){
        printf("Элемент списка типов с названием %s не найден\n", filtr_name);
        for (int i = 0; i < count_new; i++) {
            free(copy_types_filtr_name[i].name);
            if (copy_types_filtr_name[i].id_pair_domin) free(copy_types_filtr_name[i].id_pair_domin);
        }
        free(copy_types_filtr_name);
        return;
    }
    int L = index;
    while (L>0 && strcmp(copy_types_filtr_name[L-1].name, filtr_name) == 0){
        L--;
    }
    int R = index;
    while (R<count_new-1 && strcmp(copy_types_filtr_name[R+1].name, filtr_name) == 0){
        R++;
    }
    printf("===ИНФОРМАЦИЯ О НАЙДЕННЫХ ЭЛЕМЕНТАХ===\n");
    for (int i=L; i<=R; i++){
        view_one_type (copy_types_filtr_name[i]);
    }
    for (int i = 0; i < count_new; i++) {
        free(copy_types_filtr_name[i].name);
        if (copy_types_filtr_name[i].id_pair_domin) {
            free(copy_types_filtr_name[i].id_pair_domin);
        }
    }
    free(copy_types_filtr_name);
    return;
}


void filter_type_by_price(info_types inf_types){
    printf("--------------------\n");
    double filtr_price;
    while (1){
        printf("Введите СТОИМОСТЬ типа цветка (1 единица) для осуществления поиска по нему\n");

        if (scanf("%lf", &filtr_price) != 1){
            printf("Ошибка ввода стоимости типа цветка\n");
            clear_buffer();
            continue;
        }
        clear_buffer();
        break;
    }    
    printf("--------------------\n");
    // поиск всех элементов по стоимости (стоимости могут повторяться)
    // сначала создание копии массива, сортировка, далее бинарный поиск одного, расширение для поиска начала и конца, вывод всех из отрезка
    int count_new;
    type* copy_types_filtr_price = f_copy_types (&inf_types, &count_new);
    if (copy_types_filtr_price == NULL){
        printf("Ошибка создания копии массива при осуществлении бинарного поиска\n");
        return;
    }

    if (count_new == 0) {
        printf("Нет активных записей для поиска по стоимости типа\n");
        free(copy_types_filtr_price);
        return;
    }
    info_types copy_inf_types = {copy_types_filtr_price, count_new, count_new};
    sorting_process_type(copy_types_filtr_price, 0, count_new-1, compare_type_price, 1);
    int index = bin_search_type(0, count_new-1, copy_types_filtr_price, &filtr_price, comparator_price_filtr);
    if (index == -1){
        printf("Элемент списка типов со стоимостью %.3f не найден\n", filtr_price);
        for (int i = 0; i < count_new; i++) {
            free(copy_types_filtr_price[i].name);
            if (copy_types_filtr_price[i].id_pair_domin) free(copy_types_filtr_price[i].id_pair_domin);
        }
        free(copy_types_filtr_price);
        return;
    }
    int L = index;
    while (L > 0 && comparator_price_filtr(copy_types_filtr_price[L-1], &filtr_price) == 0){
        L--;
    }
    int R = index;
    while (R<count_new-1 && comparator_price_filtr(copy_types_filtr_price[R+1], &filtr_price) == 0){
        R++;
    }
    printf("===ИНФОРМАЦИЯ О НАЙДЕННЫХ ЭЛЕМЕНТАХ===\n");
    for (int i=L; i<=R; i++){
        view_one_type (copy_types_filtr_price[i]);
    }
    for (int i = 0; i < count_new; i++) {
            free(copy_types_filtr_price[i].name);
            if (copy_types_filtr_price[i].id_pair_domin) free(copy_types_filtr_price[i].id_pair_domin);
        }
    free(copy_types_filtr_price);
    return;
}

void find_start_end_otrezok(int index, pair* copy_pairs_filtr_code_domin, int count_new, int* start, int* end, int filtr_code_domin){
    int L = index;
    while (L > 0 && comparator_code_dominant_filtr(copy_pairs_filtr_code_domin[L-1], &filtr_code_domin) == 0){
        L--;
    }
    int R = index;
    while (R<count_new-1 && comparator_code_dominant_filtr(copy_pairs_filtr_code_domin[R+1], &filtr_code_domin) == 0){
        R++;
    }
    *start = L;
    *end = R;
}


void filter_pair_by_code_domin(info_pairs inf_pairs){
    printf("--------------------\n");
    int filtr_code_domin;
    while (1){
        printf("Введите КОД ДОМИНАНТА пары совместимости для осуществления поиска по нему\n");
        if (scanf("%d", &filtr_code_domin) != 1){
            printf("Ошибка ввода кода доминанта пары цветков\n");
            clear_buffer();
            continue;
        }
        clear_buffer();
        break;
    }    
    printf("--------------------\n");
    // поиск всех элементов по стоимости (стоимости могут повторяться)
    // сначала создание копии массива, сортировка, далее бинарный поиск одного, расширение для поиска начала и конца, вывод всех из отрезка
    int count_new;
    pair* copy_pairs_filtr_code_domin = f_copy_pairs (&inf_pairs, &count_new);
    if (copy_pairs_filtr_code_domin == NULL){
        printf("Ошибка создания копии массива при осуществлении бинарного поиска\n");
        return;
    }
    if (count_new == 0) {
        printf("Нет активных записей для поиска по коду доминанта\n");
        free(copy_pairs_filtr_code_domin);
        return;
    }
    info_pairs copy_inf_pairs = {copy_pairs_filtr_code_domin, count_new, count_new};
    sorting_process_pair(copy_pairs_filtr_code_domin, 0, count_new-1, compare_pair_dominant, 1);
    int index = bin_search_pair(0, count_new-1, copy_pairs_filtr_code_domin, &filtr_code_domin, comparator_code_dominant_filtr);
    if (index == -1){
        printf("Элементы списка совместимостей с доминантом с кодом %d не найдены\n", filtr_code_domin);
        free(copy_pairs_filtr_code_domin);
        return;
    }
    int L;
    int R;
    find_start_end_otrezok(index, copy_pairs_filtr_code_domin, count_new, &L, &R, filtr_code_domin);

    printf("===ИНФОРМАЦИЯ О НАЙДЕННЫХ ЭЛЕМЕНТАХ===\n");
    for (int i=L; i<=R; i++){
        view_one_pair (copy_pairs_filtr_code_domin[i]);
    }
    free(copy_pairs_filtr_code_domin);
    return;
}

void filter_pair_by_sum_cost(info_pairs inf_pairs){
    printf("--------------------\n");
    double filtr_sum_cost;
    while (1){
        printf("Введите СУММАРНУЮ СТОИМОСТЬ пары совместимости для осуществления поиска по ней\n");
        if (scanf("%lf", &filtr_sum_cost) != 1){
            printf("Ошибка ввода суммарной стоимости пары цветков\n");
            clear_buffer();
            continue;
        }
        clear_buffer();
        break;
    }    
    printf("--------------------\n");
    // поиск всех элементов по стоимости (стоимости могут повторяться)
    // сначала создание копии массива, сортировка, далее бинарный поиск одного, расширение для поиска начала и конца, вывод всех из отрезка
    int count_new;
    pair* copy_pairs_filtr_sum_cost = f_copy_pairs (&inf_pairs, &count_new);
    if (copy_pairs_filtr_sum_cost == NULL){
        printf("Ошибка создания копии массива при осуществлении бинарного поиска\n");
        return;
    }
    if (count_new == 0) {
        printf("Нет активных записей для поиска по суммарной стоимости\n");
        free(copy_pairs_filtr_sum_cost);
        return;
    }
    info_pairs copy_inf_pairs = {copy_pairs_filtr_sum_cost, count_new, count_new};
    sorting_process_pair(copy_pairs_filtr_sum_cost, 0, count_new-1, compare_pair_sumcost, 1);
    int index = bin_search_pair(0, count_new-1, copy_pairs_filtr_sum_cost, &filtr_sum_cost, comparator_sumcost_filtr);
    if (index == -1){
        printf("Элементы списка совместимостей с суммарной стоимостью %.3f не найдены\n", filtr_sum_cost);
        free(copy_pairs_filtr_sum_cost);
        return;
    }
    int L = index;
    while (L > 0 && comparator_sumcost_filtr(copy_pairs_filtr_sum_cost[L-1], &filtr_sum_cost) == 0){
        L--;
    }
    int R = index;
    while (R<count_new-1 && comparator_sumcost_filtr(copy_pairs_filtr_sum_cost[R+1], &filtr_sum_cost) == 0){
        R++;
    }
    printf("===ИНФОРМАЦИЯ О НАЙДЕННЫХ ЭЛЕМЕНТАХ===\n");
    for (int i=L; i<=R; i++){
        view_one_pair (copy_pairs_filtr_sum_cost[i]);
    }
    free(copy_pairs_filtr_sum_cost);
    return;
}