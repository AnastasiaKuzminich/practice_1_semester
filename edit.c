#include "headings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void recalc_sum_cost(info_types* inf_types, int pos_type, info_pairs* inf_pairs){
    int id_of_pair; // айди пары в массиве пар (уникальный)
    int id_oppos_pair;
    int pos_of_pair_in_pairs; // индекс(положение) пары в массиве пар
    int pos_of_oppos_pair_in_pairs; // индекс(положение) обратной пары в массиве пар
    int pos_type_suitable; // индекс (положение) совместимого типа в массиве типов
    int code_domin = inf_types->types[pos_type].code_type;
    int code_suit; 
    for (int i=0; i<inf_types->types[pos_type].count_pairs; i++){
        id_of_pair = inf_types->types[pos_type].id_pair_domin[i];
        
        pos_of_pair_in_pairs = bin_search_pair(0, inf_pairs->count_pairs-1, inf_pairs->pairs, &id_of_pair, comparator_id_pair_filtr);
        code_suit = inf_pairs->pairs[pos_of_pair_in_pairs].code_suitable;

        pos_type_suitable = bin_search_type(0, inf_types->count_types-1, inf_types->types, &code_suit, comparator_code_filtr);
        double new_cost = 1*inf_types->types[pos_type].price_item + inf_pairs->pairs[pos_of_pair_in_pairs].count_suitable * inf_types->types[pos_type_suitable].price_item;
        
        inf_pairs->pairs[pos_of_pair_in_pairs].sum_cost = new_cost;
        id_oppos_pair = inf_pairs->pairs[pos_of_pair_in_pairs].id_opposite_pair;

        pos_of_oppos_pair_in_pairs = bin_search_pair(0, inf_pairs->count_pairs-1, inf_pairs->pairs, &id_oppos_pair, comparator_id_pair_filtr);

        double new_cost_oppos = 1*inf_types->types[pos_type_suitable].price_item + inf_pairs->pairs[pos_of_oppos_pair_in_pairs].count_suitable * inf_types->types[pos_type].price_item;
        inf_pairs->pairs[pos_of_oppos_pair_in_pairs].sum_cost = new_cost_oppos;
    }
}

void update_types(info_types* inf_types, info_pairs* inf_pairs){
    view_types(inf_types);
    int code_type_edit;
    while (1){
        printf("Введите КОД типа для редактирования\n");
        if (scanf("%d", &code_type_edit) != 1){
            printf("Код типа для редактирования введен некоректно\n");
            continue;
        }
        break;
    }
    int pos_type_in_arr_types = bin_search_type(0, inf_types->count_types-1, inf_types->types, &code_type_edit, comparator_code_filtr);
    if (pos_type_in_arr_types == -1){
        printf("Тип цветка с кодом %d не найден\n", code_type_edit);
        return;
    }
    printf("Информация о типе цветка для редактирования\n\n");
    view_one_type (inf_types->types[pos_type_in_arr_types]);
    int param_to_edit;
    while (1){
        printf("Выберите параметр для изменения:\n1. Название\n2. Высота\n3. Стоимость одного цветка\n\n");
        if (scanf("%d", &param_to_edit) != 1 || param_to_edit < 1 || param_to_edit>3){
            printf("Необходимо выбрать число 1, 2 или 3\n");
            continue;
        }
        break;
    }
    switch (param_to_edit){
        case 1:{
            printf("Старое название типа цветка: %s\n", inf_types->types[pos_type_in_arr_types].name);
            clear_buffer();
            char* temp_name = NULL;
            if (receive_name(&temp_name) == 1){
                return;
            }
            
            inf_types->types[pos_type_in_arr_types].name = temp_name; 
            printf("Название цветка успешно заменено на '%s'\n", temp_name);
            return;
        }
        case 2:{
            printf("Старая высота типа цветка: %lf\n", inf_types->types[pos_type_in_arr_types].high);
            double edit_high = receive_high();
            inf_types->types[pos_type_in_arr_types].high = edit_high;
            printf("Высота типа цветка успешно заменена на '%lf'\n", edit_high);
            return;
        }
        case 3:{
            printf("Старая стоимость единицы типа цветка: %lf\n", inf_types->types[pos_type_in_arr_types].price_item);
            double edit_price = receive_price();
            inf_types->types[pos_type_in_arr_types].price_item = edit_price;
            recalc_sum_cost(inf_types, pos_type_in_arr_types, inf_pairs);
        }
    }
}

void update_pairs(info_types* inf_types, info_pairs* inf_pairs){
    view_pairs(inf_pairs);
    int id_pair_edit;
    while (1){
        printf("Введите КОД пары для редактирования\n");
        if (scanf("%d", &id_pair_edit) != 1){
            printf("Код пары для редактирования введен некоректно\n");
            continue;
        }
        break;
    }
    int pos_pair_in_pairs = bin_search_pair(0, inf_pairs->count_pairs-1, inf_pairs->pairs, &id_pair_edit, comparator_id_pair_filtr);
    if (pos_pair_in_pairs == -1){
        printf("Пары совместимости с таким кодом на найдено\n");
        return;
    }
    int code_domin = inf_pairs->pairs[pos_pair_in_pairs].code_dominant;
    int code_suit = inf_pairs->pairs[pos_pair_in_pairs].code_suitable;

    int pos_domin_in_types = bin_search_type(0, inf_types->count_types-1, inf_types->types, &code_domin, comparator_code_filtr);
    int pos_suit_in_types = bin_search_type(0, inf_types->count_types-1, inf_types->types, &code_suit, comparator_code_filtr);

    double new_count_suitable;
    while (1){
        printf("Введите новое оптимально количество цветов типа %s для одного доминантного цветка типа %s\n", inf_types->types[pos_suit_in_types].name, inf_types->types[pos_domin_in_types].name);
        if (scanf("%lf", &new_count_suitable) != 1 || new_count_suitable <=0 ){
            printf("Количество цветов введено некоректно\n");
            clear_buffer();
            continue;
        }
        clear_buffer();
        break;
    }
    
    inf_pairs->pairs[pos_pair_in_pairs].count_suitable = new_count_suitable; // Исправлено!! тест 19
    inf_pairs->pairs[pos_pair_in_pairs].sum_cost = 1*inf_types->types[pos_domin_in_types].price_item + new_count_suitable*inf_types->types[pos_suit_in_types].price_item;
    int id_oppos_pair = inf_pairs->pairs[pos_pair_in_pairs].id_opposite_pair;
    int pos_oppos_pair_in_pairs = bin_search_pair(0, inf_pairs->count_pairs-1, inf_pairs->pairs, &id_oppos_pair, comparator_id_pair_filtr);
    if (pos_oppos_pair_in_pairs == -1) {
        printf("Ошибка: обратная пара не найдена\n");
        return;
    }
    inf_pairs->pairs[pos_oppos_pair_in_pairs].count_suitable = 1/new_count_suitable;
    inf_pairs->pairs[pos_oppos_pair_in_pairs].sum_cost = 1*inf_types->types[pos_suit_in_types].price_item + inf_pairs->pairs[pos_oppos_pair_in_pairs].count_suitable*inf_types->types[pos_domin_in_types].price_item;
}