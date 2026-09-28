#include "headings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void delete_type(info_types* inf_types){
    // вывести весь список типов
    // запросить КОД типа для удаления
    // проверить, есть ли тип с таким кодом (бинарный поиск) -- получение позиции в массиве
    // проверить длину массива индексов пар (должна быть равна 0)
    // присвоить флаг как удалено
    view_types(inf_types);
    int code_type_delete;
    while (1){
        printf("Введите КОД типа цветка для удаления по нему\n");
        if (scanf("%d", &code_type_delete) != 1){
            printf("Неверно введен код типа цветка\n");
            clear_buffer();
            continue;
        }
        clear_buffer();
        break;
    }
    int ind_type_to_del = bin_search_type(0, inf_types->count_types-1, inf_types->types, &code_type_delete, comparator_code_filtr);
    if (ind_type_to_del == -1){
        printf("Такой цветок с кодом %d не найден\n", code_type_delete);
        return;
    }
    if (inf_types->types[ind_type_to_del].count_pairs != 0){
        printf("Удаление типа цветка не может быть выполнено в связи с наличием пар совместимостей количеством %d\n", inf_types->types[ind_type_to_del].count_pairs);
        return;
    }
    // ИСПРАВЛЕНИЕ. ТЕСТ 12.
    if (inf_types->types[ind_type_to_del].is_deleted == 1){
        printf("Тип цветка с таким кодом уже был удален.\n");
        return;
    }
    inf_types->types[ind_type_to_del].is_deleted = 1;
    printf("Тип цветка **%s** успешно удален!!\n", inf_types->types[ind_type_to_del].name);
}

void remove_id_pair_from_arr_type(info_types* inf_types, int id_to_del, int pos_of_type){
    int pos_last = inf_types->types[pos_of_type].count_pairs -1 ;
    for (int i=0; i<=pos_last; i++){
        if (inf_types->types[pos_of_type].id_pair_domin[i] == id_to_del){
            if (i == pos_last){
                inf_types->types[pos_of_type].count_pairs --;//затерли последний элемент
                return;
            }
            inf_types->types[pos_of_type].id_pair_domin[i] = inf_types->types[pos_of_type].id_pair_domin[pos_last]; // поставили на это место последний айди
            inf_types->types[pos_of_type].count_pairs --;
            return;
        }
    }
}

void delete_pair_and_opposite(info_pairs* inf_pairs, info_types* inf_types){
    view_pairs(inf_pairs);
    int id_pair_delete;
    while (1){
        printf("Введите КОД ПАРЫ совместимости для удаления по нему\n");
        if (scanf("%d", &id_pair_delete) != 1){
            printf("Неверно введен код пары совместимости\n");
            clear_buffer();
            continue;
        }
        clear_buffer();
        break;
    }
    int ind_pair_to_delete = bin_search_pair(0, inf_pairs->count_pairs-1, inf_pairs->pairs, &id_pair_delete, comparator_id_pair_filtr);
    if (ind_pair_to_delete == -1 ){
        printf("Пара совместимости с кодом %d не найдена\n", id_pair_delete);
        return;
    }
    int ind_opposite_pair_to_delete = bin_search_pair(0, inf_pairs->count_pairs-1, inf_pairs->pairs, &inf_pairs->pairs[ind_pair_to_delete].id_opposite_pair, comparator_id_pair_filtr); 
    int id_opposite_pair_delete = inf_pairs->pairs[ind_pair_to_delete].id_opposite_pair;
    if (ind_opposite_pair_to_delete == -1){
        printf("Обратная пара совместимости с кодом %d не найдена\n", id_opposite_pair_delete);
        return;
    }
    int code_domin = inf_pairs->pairs[ind_pair_to_delete].code_dominant;
    int code_suitable = inf_pairs->pairs[ind_opposite_pair_to_delete].code_dominant;    
    int pos_type_of_domin_in_types = bin_search_type(0, inf_types->count_types-1, inf_types->types, &code_domin, comparator_code_filtr);
    int pos_type_of_suitable_in_types = bin_search_type(0, inf_types->count_types-1, inf_types->types, &code_suitable, comparator_code_filtr);

    if (pos_type_of_domin_in_types == -1 || pos_type_of_suitable_in_types == -1){
        printf("Ошибка разрушения связей при удалении пары\n");
        return;
    }
    remove_id_pair_from_arr_type(inf_types, id_pair_delete, pos_type_of_domin_in_types);
    remove_id_pair_from_arr_type(inf_types, id_opposite_pair_delete, pos_type_of_suitable_in_types);

    inf_pairs->pairs[ind_pair_to_delete].is_deleted = 1;
    inf_pairs->pairs[ind_opposite_pair_to_delete].is_deleted = 1;
    printf("Пара совместимости с кодом %d успешно удалена!\n", ind_pair_to_delete+1);
    printf("Обратная пара совместимости с кодом %d успешно удалена!\n", ind_opposite_pair_to_delete+1);
    return;
}