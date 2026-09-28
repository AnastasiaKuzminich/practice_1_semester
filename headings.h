#ifndef HEADINGS_H
#define HEADINGS_H
#include <stdio.h>
#define LEN_NAME 50
#define CAPACITY_COMBINATIONS 10

typedef struct{
    int code_type; // FIND Уникальный код
    char* name; //SORT  FIND   **EDIT     Название (динамическая строка)
    double high; //SORT     **EDIT   Высота
    double price_item; // SORT    FIND  **EDIT    Цена за 1 шт.
    int* id_pair_domin; // массив id_paid, для которых этот тип цветка является доминантным
    int count_pairs;     // Количество таких пар
    int capacity_pairs; //вместимость массива id пар
    int is_deleted;//0-exist 1-deleted
}type;

typedef struct{
    type* types; 
    int count_types;
    int capacity_types;
}info_types;

typedef struct {
    int id_pair; // Уникальный ID пары
    int code_dominant; //SORT   FIND Код доминирующего цветка
    int code_suitable; // Код совместимого цветка
    double count_suitable; // **EDIT Оптимальное количество
    double sum_cost; // SORT  FIND   Суммарная стоимость (рассчитываемая) 
    int id_opposite_pair; // ID обратной пары
    int is_deleted; //0-exist 1-deleted
}pair;

typedef struct{
    pair* pairs;
    int count_pairs;
    int capacity_pairs;
}info_pairs;

typedef struct{
    int* indexes; 
    int count_indexes;
    int capacity_indexes;
}combination_indexes;



void clear_buffer();


int choose_spisok();

int load_types(const char* fname, info_types* inf_types);
int load_pairs(const char* fname, info_pairs* inf_pairs);

int save_types(const char* fname, info_types* inf_types);
int save_pairs(const char* fname, info_pairs* inf_pairs);


void view_one_type (type one_type);
void view_one_pair (pair one_pair);

void view_types(info_types* inf_types);
void view_pairs(info_pairs* inf_pairs);

int compare_type_name(type a, type b, int dir);
int compare_type_height(type a, type b, int dir);
int compare_type_price(type a, type b, int dir);
int compare_pair_dominant(pair a, pair b, int dir);
int compare_pair_sumcost(pair a, pair b, int dir);

void sorting_process_type(type* array,  int left, int right, int (*comparator)(type, type, int ), int dir);
void sorting_process_pair(pair* array,  int left, int right, int (*comparator)(pair, pair, int), int dir);

type* f_copy_types (info_types* inf_types, int* count_new);
pair* f_copy_pairs (info_pairs* inf_pairs, int* count_new);

int sort_types(info_types* inf_types, int (*comparator) (type, type, int), int dir);
int sort_pairs(info_pairs* inf_pairs, int (*comparator) (pair, pair, int), int dir);

void free_types(info_types* inf_types);
void free_pairs(info_pairs* inf_pairs);


int comparator_code_filtr(type a, void* b);
int comparator_name_filtr(type a, void* b);
int comparator_price_filtr(type a, void* b);
int comparator_code_dominant_filtr(pair a, void* b);
int comparator_sumcost_filtr(pair a, void* b);
//*
int comparator_id_pair_filtr(pair a, void* b);


int bin_search_type(int L, int R, type* types, void* kriterij, int (*comparator)(type a, void* b));
int bin_search_pair(int L, int R, pair* pairs, void* kriterij, int (*comparator)(pair a, void* b));

void filter_type_by_code(info_types inf_types);
void filter_type_by_name(info_types inf_types);
void filter_type_by_price(info_types inf_types);
void filter_pair_by_code_domin(info_pairs inf_pairs);
void filter_pair_by_sum_cost(info_pairs inf_pairs);

int get_next_code(info_types* inf_types);
void add_type(info_types* inf_types);

int if_such_pair_exist(info_pairs* inf_pairs, int add_code_domin, int add_code_suitable);
int get_next_id_pair(info_pairs* inf_pairs);
void add_pair(info_pairs* inf_pairs, info_types* inf_types);
int expand_memory_for_ids_in_arr_types(info_types* inf_types, int ind);
void add_info_to_new_pair(info_pairs* inf_pairs, int new_place, int new_id, int add_code_domin, int add_code_suitable, double new_count_suitable, double new_sum_cost);

int receive_name(char** temp);
double receive_high();
double receive_price();


void delete_type(info_types* inf_types);
void remove_id_pair_from_arr_type(info_types* inf_types, int id_to_del, int pos_of_type);
void delete_pair_and_opposite(info_pairs* inf_pairs, info_types* inf_types);

void recalc_sum_cost(info_types* inf_types, int pos_type, info_pairs* inf_pairs);
void update_types(info_types* inf_types, info_pairs* inf_pairs);
void update_pairs(info_types* inf_types, info_pairs* inf_pairs);

void get_buket_input(info_types* inf_types, double* budget, int* count_types, int* code_type);
int validate_input(info_types* inf_types, double budget, int count_types, int code_type_main);
int* find_idx_of_total_live_types(info_types* inf_types, int* total_live_types);
int are_compatible(info_types* inf_types, info_pairs* inf_pairs, int pos_domin, int code_suit);// 0-no 1-yes
int is_compatible_with_current_set(int candidate_idx, combination_indexes* combinations, info_types* inf_types, info_pairs* inf_pairs);
double get_count_suitable(info_types* inf_types, info_pairs* inf_pairs, int pos_domin, int code_suit);
int process_and_print_bouquet(combination_indexes* combinations, info_types* inf_types, info_pairs* inf_pairs, double budget, FILE* res_file);
int start_recursion(int start_index, int need_count, combination_indexes* combinations, int* live_types_idx, int total_live_types, int domin_type_idx, info_types* inf_types, info_pairs* inf_pairs, double budget, FILE* res_file) ;
int init_arr_combinations(combination_indexes* combinations);
int extend_arr_combinations(combination_indexes* combinations);

void special_fun(info_types* inf_types, info_pairs* inf_pairs);

#endif