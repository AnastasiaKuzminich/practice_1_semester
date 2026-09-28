#include <stdio.h>
#include "headings.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>


void get_buket_input(info_types* inf_types, double* budget, int* count_types, int* code_type){
    while (1){
        printf("Бюджет:\n");
        if (scanf("%lf", budget) != 1){
            printf("Некорректный ввод значения бюджета\n");
            clear_buffer();
            continue;
        }
        clear_buffer();
        break;
    }
    while (1){
        printf("Количество типов цветов:\n");
        if (scanf("%d", count_types) != 1){
            printf("Некорректный ввод количества типов цветов\n");
            clear_buffer();
            continue;
        }
        clear_buffer();
        break;
    }
    printf("Выберите из предложенного списка один тип цветка, который обязательным\n");
    view_types(inf_types);
    while (1){
        printf("Код выбранного типа цветка:\n");
        if (scanf("%d", code_type) != 1){
            printf("Некорректный ввод кода типа цветка\n");
            clear_buffer();
            continue;
        }
        clear_buffer();
        break;
    }
}

int validate_input(info_types* inf_types, double budget, int count_types, int code_type_main){
    if (budget < 0){
        printf("Бюджет отрицательный. Составление букетов невозможно\n");
        return 1;
    }
    if (count_types < 1 || count_types>inf_types->count_types){
        printf("Число типов цветов должно быть больше 0 и меньше или равно числу всех типов цветков\n");
        return 1;
    }
    int pos_main_type_in_types = bin_search_type(0, inf_types->count_types-1, inf_types->types, &code_type_main, comparator_code_filtr);
    if (pos_main_type_in_types == -1){
        printf("Тип цветка с кодом %d не найден. Без обязательного типа букет не может быть собран..\n", code_type_main);
        return 1;
    }
    return 0;
}

int* find_idx_of_total_live_types(info_types* inf_types, int* total_live_types){
    int* live_types_idx = malloc(inf_types->count_types * sizeof(int));
    *total_live_types = 0;
    for (int i = 0; i < inf_types->count_types; i++){
        if (inf_types->types[i].is_deleted == 0){
            live_types_idx[*total_live_types] = i;
            (*total_live_types)++;
        }
    }
    return live_types_idx;
}

int are_compatible(info_types* inf_types, info_pairs* inf_pairs, int pos_domin, int code_suit){// 0-no 1-yes
    int count_pair = inf_types->types[pos_domin].count_pairs;
    if (count_pair == 0)  return 0;
    for (int i = 0; i<count_pair; i++){ // по каждoй паре совместимсоти, где первый код является доминнатом
        int targed_id = inf_types->types[pos_domin].id_pair_domin[i];
        int pos_pair = bin_search_pair(0, inf_pairs->count_pairs-1, inf_pairs->pairs, &targed_id, comparator_id_pair_filtr);
        if (pos_pair == -1){
            continue;
        }
        if (inf_pairs->pairs[pos_pair].code_suitable == code_suit){
            return 1;
        }
    }
    return 0;
}


int is_compatible_with_current_set(int candidate_idx, combination_indexes* combinations, info_types* inf_types, info_pairs* inf_pairs) {
    for (int i = 0; i < combinations->count_indexes; i++) {
        int already_in_buket_idx = combinations->indexes[i];
        if (are_compatible(inf_types, inf_pairs, candidate_idx, inf_types->types[already_in_buket_idx].code_type) == 0 ) {
            return 1; // Не совместимы
        }
    }
    return 0; // Совместимы
}

double get_count_suitable(info_types* inf_types, info_pairs* inf_pairs, int pos_domin, int code_suit) {
    int count_pair = inf_types->types[pos_domin].count_pairs;
    for (int i = 0; i < count_pair; i++) {
        int id_pair = inf_types->types[pos_domin].id_pair_domin[i];
        int pos_pair = bin_search_pair(0, inf_pairs->count_pairs - 1, inf_pairs->pairs, &id_pair, comparator_id_pair_filtr);
        
        if (pos_pair != -1 && inf_pairs->pairs[pos_pair].code_suitable == code_suit) {
            return inf_pairs->pairs[pos_pair].count_suitable;
        }
    }
    return 0; 
}

int process_and_print_bouquet(combination_indexes* combinations, info_types* inf_types, info_pairs* inf_pairs, double budget, FILE* res_file){
    int count_main_flower = 1;
    int domin_idx = combinations->indexes[0];
    int printed = 0;
    while (1){
        double real_sum = 0; // Итоговая сумма с учетом округления
        real_sum += inf_types->types[domin_idx].price_item * count_main_flower;
        for (int i = 1; i < combinations->count_indexes; i++){
            int suit_idx = combinations->indexes[i];
            double r = get_count_suitable(inf_types, inf_pairs, domin_idx, inf_types->types[suit_idx].code_type);
            
            double rounded_count = ceil(count_main_flower * r);
            
            real_sum += inf_types->types[suit_idx].price_item * rounded_count;
        }
        if (real_sum > budget) {
            break; 
        }
        printed = 1;
        printf("\n--- Предложенный вариант букета ---\n");
        printf("Бюджет: %.2f руб. | Текущая стоимость: %.2f руб.\n", budget, real_sum);
        printf("1. %s (ОСНОВНОЙ): %d шт.\n", inf_types->types[domin_idx].name, count_main_flower);    
        for (int i = 1; i < combinations->count_indexes; i++) {
            int s_idx = combinations->indexes[i];
            double r = get_count_suitable(inf_types, inf_pairs, domin_idx, inf_types->types[s_idx].code_type);
            double rounded_count_suit = ceil(count_main_flower * r);
            printf("%d. %s: %.0f шт.\n", i + 1, inf_types->types[s_idx].name, rounded_count_suit);
        }
        printf("-----------------------------------\n");  
        if (res_file != NULL) {
            fprintf(res_file, "\n--- Предложенный вариант букета ---\n");
            fprintf(res_file, "Бюджет: %.2f руб. | Текущая стоимость: %.2f руб.\n", budget, real_sum);
            fprintf(res_file, "1. %s (ОСНОВНОЙ): %d шт.\n", inf_types->types[domin_idx].name, count_main_flower);
            for (int i = 1; i < combinations->count_indexes; i++) {
                int s_idx = combinations->indexes[i];
                double r = get_count_suitable(inf_types, inf_pairs, domin_idx, inf_types->types[s_idx].code_type);
                double rounded_count_suit = ceil(count_main_flower * r);
                fprintf(res_file, "%d. %s: %.0f шт.\n", i + 1, inf_types->types[s_idx].name, rounded_count_suit);
            }
            fprintf(res_file, "-----------------------------------\n");
        }
        count_main_flower += 2; // Увеличиваем размер букета
    }
    return printed;
}

int start_recursion(int start_index, int need_count, combination_indexes* combinations, int* live_types_idx, int total_live_types, int domin_type_idx, info_types* inf_types, info_pairs* inf_pairs, double budget, FILE* res_file) {
    int found_at_least_one = 0;
    // БАЗОВЫЙ СЛУЧАЙ: собрано нужное количество типов
    if (combinations->count_indexes == need_count) {
        return process_and_print_bouquet(combinations, inf_types, inf_pairs, budget, res_file);
    }
    // Перебор всех доступных цветов, начиная со start_index
    for (int i = start_index; i < total_live_types; i++) {
        int candidate_type_idx = live_types_idx[i];
        if (candidate_type_idx == domin_type_idx) continue;
        if (is_compatible_with_current_set(candidate_type_idx, combinations, inf_types, inf_pairs) == 0) { //1-не совместимы 0-совместимы
            if (combinations->count_indexes >= combinations->capacity_indexes) {
                if (extend_arr_combinations(combinations) == 1) {
                    printf("Ошибка выделения памяти\n");
                    free(live_types_idx);
                    free(combinations->indexes);
                    if (res_file != NULL) fclose(res_file);
                    return found_at_least_one; // Прерываемся, если память не выделилась
                }
            }
            combinations->indexes[combinations->count_indexes] = candidate_type_idx;
            combinations->count_indexes++;

            if (start_recursion(i+1, need_count, combinations, live_types_idx, total_live_types, domin_type_idx , inf_types, inf_pairs, budget, res_file) == 1)  {
                found_at_least_one = 1;
            }
            combinations->count_indexes--;
        }
    }
    return found_at_least_one;
}

int init_arr_combinations(combination_indexes* combinations){
    combinations->count_indexes = 0;
    combinations->capacity_indexes = CAPACITY_COMBINATIONS;
    combinations->indexes = malloc(combinations->capacity_indexes *sizeof(int));
    if (combinations->indexes == NULL){
        printf("Ошибка выделения памяти при составлении букетов\n");
        return 1;
    }
    return 0;
}

int extend_arr_combinations(combination_indexes* combinations){
    combinations->capacity_indexes = combinations->capacity_indexes * 2;
    combinations->indexes = realloc(combinations->indexes, sizeof(int) * combinations->capacity_indexes);
    if (combinations->indexes == NULL){
        printf("Ошибка выделения памяти при составлении букетов_2\n");
        return 1;
    }
    return 0;
}

void special_fun(info_types* inf_types, info_pairs* inf_pairs) {
    double budget;
    int need_count;
    int code_type_main;
    get_buket_input(inf_types, &budget, &need_count, &code_type_main);

    if (validate_input(inf_types, budget, need_count, code_type_main) == 1){
        return;
    }
    printf("Данные для составления букета введены корректно!\n");
    FILE* res_file = fopen("bukets_report.txt", "a");
    if (res_file == NULL) {
        printf("Предупреждение: Не удалось создать файл отчета. Данные будут выведены только на экран.\n");
    } else {
        fprintf(res_file, "=== ОТЧЕТ О СФОРМИРОВАННЫХ БУКЕТАХ ===\n");
    }

    combination_indexes combinations;
    if (init_arr_combinations(&combinations) == 1) return;
    int found = 0;
    int total_live_types;
    int* live_types_idx = find_idx_of_total_live_types(inf_types, &total_live_types);
    // Обязательный цветок всегда на первом месте
    int domin_type_idx = bin_search_type(0, inf_types->count_types-1, inf_types->types, &code_type_main, comparator_code_filtr);
 
    for (int temp_count_types = need_count; temp_count_types >= 1; temp_count_types--) {
        combinations.count_indexes = 1;
        combinations.indexes[0] = domin_type_idx;
        // рекурсию с 0 индекса списка 
        // в корзине уже 1 цветок (обязательный)
        found = start_recursion(0, temp_count_types, &combinations, live_types_idx, total_live_types, domin_type_idx, inf_types, inf_pairs, budget, res_file);
        if (found == 1) {
            if (temp_count_types < need_count) {
                if (res_file != NULL) {
                    fprintf(res_file, "Букет из %d типов невозможен. Составлен из %d типов.\n", need_count, temp_count_types);
                } 
                printf("Букет из %d типов невозможен. Составлен из %d типов.\n", need_count, temp_count_types);
            }
            break; // букет составлен из максимального числа типа цветков (как ввел пользователь)
        }
    }
    if (found == 0){
        if (res_file != NULL) {
            fprintf(res_file, "К сожалению, по вашим параметрам не удалось подобрать ни одного сочетания цветов\n");
        }
        printf("К сожалению, по вашим параметрам не удалось подобрать ни одного сочетания цветов\n");
    }

    if (res_file != NULL) {
        fclose(res_file);
        printf("Результаты успешно сохранены в файл 'bukets_report.txt'\n");
    }
    free(live_types_idx);
    free(combinations.indexes);
}

