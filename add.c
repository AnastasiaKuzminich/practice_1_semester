#include "headings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_next_code(info_types* inf_types){
    if (inf_types->count_types == 0) return 1;
    return inf_types->types[inf_types->count_types-1].code_type + 1;
}

int receive_name(char** temp){
    char name[LEN_NAME];
    while (1){
        printf("Введите НАЗВАНИЕ типа цветка\n");
        if (fgets(name, LEN_NAME, stdin) == NULL){
            printf("Ошибка ввода названия типа цветка\n");
            continue;
        }
        name[strcspn(name, "\n")] = '\0';
        break;
    } 
    *temp = strdup(name);
    if (*temp == NULL) { 
        printf("Ошибка выделения памяти для названия типа цветка\n");
        return 1;
    }
    return 0;
}

double receive_high(){
    double high;
    while (1){
        printf("Введите ВЫСОТУ типа цветка\n");
        if (scanf("%lf", &high) != 1 || high<=0){
            printf("Ошибка ввода высоты типа цветка\n");
            clear_buffer(); // ИСПРАВЛЕНО!!! 
            continue;
        }
        clear_buffer();
        break;
    }
    return high;
}

double receive_price(){
    double price;
    while (1){
        printf("Введите СТОИМОСТЬ типа цветка\n");
        
        if (scanf("%lf", &price) != 1 || price<0 ){
            printf("Ошибка ввода стоимости типа цветка\n");
            clear_buffer(); // ИСПРАВЛЕНО!!!
            continue;
        }
        clear_buffer();
        break;
    } 
    return price;
}


void add_type(info_types* inf_types){
    if (inf_types->count_types == inf_types->capacity_types){
        int new_capacity = (inf_types->capacity_types == 0) ? 50 : (int)(inf_types->capacity_types * 1.1) + 1;
        type* temp = realloc (inf_types->types, new_capacity*sizeof(type));
        if (temp == NULL){
            printf("Ошибка выделения памяти\n");
            return;
        }
        inf_types->types = temp;
        inf_types->capacity_types = new_capacity;
    }
    int new_pos = inf_types->count_types;
    int next_code = get_next_code(inf_types);
    inf_types->types[new_pos].code_type = next_code;
    printf("----------\n");
    char* temp_name = NULL;
    if (receive_name(&temp_name) == 1){
        return;
    }
    inf_types->types[new_pos].name = temp_name; 
    double add_high = receive_high();
    inf_types->types[new_pos].high = add_high;
    double add_price = receive_price();
    inf_types->types[new_pos].price_item = add_price;
    printf("----------\n");
    inf_types->types[new_pos].id_pair_domin = NULL;
    inf_types->types[new_pos].is_deleted = 0;
    inf_types->types[new_pos].count_pairs = 0;
    inf_types->types[new_pos].capacity_pairs = 0; // Изначально массив id пуст
    (inf_types->count_types) ++;
    printf("Элемент с уникальным кодом типа %d успешно добавлен!!\n\n", next_code);
}

int if_such_pair_exist(info_pairs* inf_pairs, int add_code_domin, int add_code_suitable) {
    // Если массива еще нет или в нем 0 элементов — пары точно не существует
    if (inf_pairs->pairs == NULL || inf_pairs->count_pairs == 0) {
        return 0; 
    }
    //проход по всем существующим парам
    for (int i = 0; i < inf_pairs->count_pairs; i++) {
        // Проверяем только те, что не удалены
        if (inf_pairs->pairs[i].is_deleted == 0) {
            if (inf_pairs->pairs[i].code_dominant == add_code_domin && 
                inf_pairs->pairs[i].code_suitable == add_code_suitable) {
                return 1; // Такая пара уже есть в базе
            }
        }
    }
    return 0; 
}

int get_next_id_pair(info_pairs* inf_pairs){
    if (inf_pairs->count_pairs == 0) return 1;
    return inf_pairs->pairs[inf_pairs->count_pairs-1].id_pair + 1;
}

void add_pair(info_pairs* inf_pairs, info_types* inf_types){
    printf("Список типов цветов для выбора двух типов для создания пары\n\n\n");
    view_types(inf_types);
    printf("===========================================================\n");
    int add_code_domin;
    while (1){
        printf("Введите код цветка-доминанта\n");
        if (scanf("%d", &add_code_domin) != 1 || add_code_domin < 1){
            printf("Неверный ввод кода цветка\n");
            clear_buffer();
            continue;
        }
        clear_buffer();
        break;
    }
    int add_code_suitable;
    while (1){
        printf("Введите код цветка-совместимого\n");
        if (scanf("%d", &add_code_suitable) != 1 || add_code_suitable < 1|| add_code_suitable == add_code_domin){
            printf("Неверный ввод кода цветка\n");
            clear_buffer();
            continue;
        }
        clear_buffer();
        break;
    }
    int ind_dom = bin_search_type(0, inf_types->count_types-1, inf_types->types, &add_code_domin, comparator_code_filtr);
    int ind_suit = bin_search_type(0, inf_types->count_types-1, inf_types->types, &add_code_suitable, comparator_code_filtr);
    if (ind_dom == -1 ){
        printf("Цветок - доминант с таким кодом не найден\n");
        return;
    } 
    if (ind_suit == -1 ){
        printf("Цветок - совместимый с таким кодом не найден\n");
        return;
    } 
    int if_exist_pair = if_such_pair_exist(inf_pairs, add_code_domin, add_code_suitable);
    if (if_exist_pair == 2){
        printf("Пара не может быть добавлена. Повторите попытку...\n");
        return;
    }
    if (if_exist_pair == 1){
        printf("Такая пара цветков уже существует. Добавление ее невозможно\n");
        return;
    }
    if (expand_memory_for_ids_in_arr_types(inf_types, ind_dom) == 1 || 
        expand_memory_for_ids_in_arr_types(inf_types, ind_suit) == 1){
        printf("Ошибка выделения динамической памяти в структурах типов.\n");
        return;
    }
    if (inf_pairs->count_pairs + 2 >= inf_pairs->capacity_pairs){
        int new_capacity = (inf_pairs->capacity_pairs == 0) ? 50 : (int)(inf_pairs->capacity_pairs * 1.5) + 2;
        pair* temp = realloc(inf_pairs->pairs, new_capacity * sizeof(pair));
        if (temp == NULL){
            printf("Ошибка выделения памяти под новые пары\n");
            return;
        }
        inf_pairs->pairs = temp;
        inf_pairs->capacity_pairs = new_capacity;
    }

    int new_id = get_next_id_pair(inf_pairs);
    double new_count_suitable;
    while (1){
        printf("Введите оптимальное количество цветков совместимого типа для одного доминирующего (возможно вещественное значение)\n");

        if (scanf("%lf", &new_count_suitable) != 1 || new_count_suitable <= 0){
            printf("Ошибка ввода количества цветков\n");
            clear_buffer();
            continue;
        }
        clear_buffer();
        break;
    }     
    int new_place = inf_pairs->count_pairs;
    
    int new_place_opposite = inf_pairs->count_pairs + 1;
    int new_id_opposite = new_id+1;
    double cost_direct = 1 * inf_types->types[ind_dom].price_item + new_count_suitable * inf_types->types[ind_suit].price_item;
    double cost_opposite = inf_types->types[ind_dom].price_item / new_count_suitable + 1 * inf_types->types[ind_suit].price_item;
    add_info_to_new_pair(inf_pairs, new_place, new_id, add_code_domin, add_code_suitable, new_count_suitable, cost_direct);
    add_info_to_new_pair(inf_pairs, new_place_opposite, new_id_opposite, add_code_suitable, add_code_domin, 1/new_count_suitable, cost_opposite);
    inf_pairs->count_pairs += 2;
    inf_pairs->pairs[new_place].id_opposite_pair=new_id_opposite;
    inf_pairs->pairs[new_place_opposite].id_opposite_pair=new_id;
    int new_pos_id_pair_in_type = inf_types->types[ind_dom].count_pairs;
    inf_types->types[ind_dom].id_pair_domin[new_pos_id_pair_in_type] = new_id;
    inf_types->types[ind_dom].count_pairs ++;
    printf("Для пары совместимости с id = %d код доминанта %d добавлен в структуру типа цветка\n", new_id, add_code_domin);
    int new_pos_id_oppos_pair_in_type = inf_types->types[ind_suit].count_pairs;
    inf_types->types[ind_suit].id_pair_domin[new_pos_id_oppos_pair_in_type] = new_id_opposite;
    inf_types->types[ind_suit].count_pairs ++;
    printf("Для пары совместимости с id = %d код доминанта %d добавлен в структуру типа цветка\n", new_id_opposite, add_code_suitable);
}

int expand_memory_for_ids_in_arr_types(info_types* inf_types, int ind){
    if (inf_types->types[ind].count_pairs == inf_types->types[ind].capacity_pairs){
        int new_capacity = (inf_types->types[ind].capacity_pairs == 0) ? 50 : (int)(inf_types->types[ind].capacity_pairs * 1.1) + 1;
        int* temp = NULL;
        if (inf_types->types[ind].id_pair_domin == NULL || inf_types->types[ind].capacity_pairs == 0) {
            temp = malloc(new_capacity * sizeof(int));
        } else {
            temp = realloc(inf_types->types[ind].id_pair_domin, new_capacity * sizeof(int));
        }        
        if (temp == NULL){
            printf("Ошибка выделения памяти\n");
            return 1;
        }
        inf_types->types[ind].id_pair_domin = temp;
        inf_types->types[ind].capacity_pairs = new_capacity;
    }

    return 0;
}

void add_info_to_new_pair(info_pairs* inf_pairs, int new_place, int new_id, int add_code_domin, int add_code_suitable, double new_count_suitable, double new_sum_cost){ 
    
    inf_pairs->pairs[new_place].id_pair = new_id;
    inf_pairs->pairs[new_place].code_dominant = add_code_domin;
    inf_pairs->pairs[new_place].code_suitable = add_code_suitable;
    inf_pairs->pairs[new_place].sum_cost = new_sum_cost;
    inf_pairs->pairs[new_place].count_suitable = new_count_suitable;
    inf_pairs->pairs[new_place].id_opposite_pair = -1; // пока обратная пара не создана
    inf_pairs->pairs[new_place].is_deleted = 0; 
}
