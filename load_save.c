#include "headings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int load_types(const char* fname, info_types* inf_types){
    FILE*  file = fopen(fname, "rb");
    if (!file) return 1;
    // данные в файле:
    //count_types
    // блоки (count_types раз) (code_type, name, high, price_item, count_pairs, !!!capacity_pairs!!! 
    //(числа количеством count_pairs, , то есть те самые id пар))
    int count_types;
    fread(&count_types, sizeof(int), 1, file);
    inf_types->count_types = count_types;
    inf_types->capacity_types = (int)(count_types*1.1)+1;
    inf_types->types = malloc(inf_types->capacity_types*sizeof(type));
    if (!inf_types->types){
        printf("Ошибка выделения памяти для массива типов\n");
        fclose(file);
        return 1;
    }   
    int len_name;
    int count_pairs;
    for (int i = 0; i<count_types; i++){
        fread(&inf_types->types[i].code_type, sizeof(int), 1,  file);

        fread(&len_name, sizeof(int), 1,  file);
        inf_types->types[i].name = malloc((len_name+1)*sizeof(char));
        if (inf_types->types[i].name == NULL){
            printf("Ошибка выделения памяти для названия цветка номер %d\n", i);
            // очистить память массива types и строки, которые успели создать
            for (int j = 0; j <= i; j++) {
                if (inf_types->types[j].name) free(inf_types->types[j].name);
                if (inf_types->types[j].id_pair_domin) free(inf_types->types[j].id_pair_domin);
            }
            free(inf_types->types);
            fclose(file);
            return 1;
        }
        fread(inf_types->types[i].name, sizeof(char), len_name, file);
        inf_types->types[i].name[len_name] = '\0';
        fread(&inf_types->types[i].high, sizeof(double), 1,  file);
        fread(&inf_types->types[i].price_item, sizeof(double), 1,  file);
        

        fread(&count_pairs, sizeof(int), 1,  file);
        inf_types->types[i].count_pairs = count_pairs;
        fread(&inf_types->types[i].capacity_pairs, sizeof(int), 1,  file);
        
        if (count_pairs > 0) {
            if (inf_types->types[i].capacity_pairs < count_pairs) {
                inf_types->types[i].capacity_pairs = count_pairs;
            }
            
            inf_types->types[i].id_pair_domin = malloc(inf_types->types[i].capacity_pairs * sizeof(int));
            if (inf_types->types[i].id_pair_domin == NULL) {
                printf("Ошибка выделения памяти для массива пар %d, где этот цветок доминирует\n", i);
                // очистить память массива types
                // очистить все строки с именами
                for (int j = 0; j <= i; j++) {
                    if (inf_types->types[j].name) free(inf_types->types[j].name);
                    if (inf_types->types[j].id_pair_domin) free(inf_types->types[j].id_pair_domin);
                }
                free(inf_types->types);
                fclose(file);
                return 1;
            }
            fread(inf_types->types[i].id_pair_domin, sizeof(int), count_pairs, file);
        } else {
            // указатель и сбрасываем емкость в 0, чтобы realloc/malloc отрабатывали штатно
            inf_types->types[i].id_pair_domin = NULL;
            inf_types->types[i].capacity_pairs = 0; 
        }
    }
    fclose(file);
    return 0;
}
int load_pairs(const char* fname, info_pairs* inf_pairs){
    FILE*  file = fopen(fname, "rb");
    if (!file) return 1;
    // данные в файле:
    //count_pairs
    // блоки (count_pairs раз) (id_pair, code_dominant, code_suitable, count_suitable, 
    //sum_cost, id_opposite_pair)
    int count_pairs;
    fread(&count_pairs, sizeof(int), 1, file);

    int capacity_pairs = (int)(count_pairs * 1.1) + 1;
    pair* temp_pairs = malloc(capacity_pairs * sizeof(pair));
    if (!temp_pairs){
        printf("Ошибка выделения памяти для массива совместимостей\n");
        fclose(file);
        return 1; // Структура inf_pairs осталась нетронутой
    }

    for (int i = 0; i<count_pairs; i++){
        fread(&temp_pairs[i].id_pair, sizeof(int), 1,  file);
        fread(&temp_pairs[i].code_dominant, sizeof(int), 1,  file);
        fread(&temp_pairs[i].code_suitable, sizeof(int), 1,  file);
        fread(&temp_pairs[i].count_suitable, sizeof(double), 1,  file);
        fread(&temp_pairs[i].sum_cost, sizeof(double), 1,  file);
        fread(&temp_pairs[i].id_opposite_pair, sizeof(int), 1,  file);
        temp_pairs[i].is_deleted = 0;
    }
    inf_pairs->count_pairs = count_pairs;
    inf_pairs->capacity_pairs = capacity_pairs;
    inf_pairs->pairs = temp_pairs;

    fclose(file);
    return 0;
}

int f_count_life_types(info_types* inf_types){
    int count = 0;
    for (int i=0; i<inf_types->count_types; i++){
        if (inf_types->types[i].is_deleted == 0){
            count++;
        }
    }
    return count;
}

int f_count_life_pairs(info_pairs* inf_pairs){
    int count = 0;
    for (int i=0; i<inf_pairs->count_pairs; i++){
        if (inf_pairs->pairs[i].is_deleted == 0){
            count++;
        }
    }
    return count;
}

int save_types(const char* fname, info_types* inf_types){
    FILE* file = fopen(fname, "wb");
    if (!file){
        printf("Ошибка сохранения данных о типах в файл\n");
        return 1;
    }
    int count_life_types = f_count_life_types(inf_types);
    fwrite(&count_life_types, sizeof(int), 1, file);
    for (int i = 0; i<inf_types->count_types; i++){
        if (inf_types->types[i].is_deleted != 1){
            fwrite(&inf_types->types[i].code_type, sizeof(int), 1, file);
            int len_name = (int)strlen(inf_types->types[i].name);
            fwrite(&len_name, sizeof(int), 1,  file);
            if (len_name>0){
                fwrite(inf_types->types[i].name, sizeof(char), len_name, file);
            }
            fwrite(&inf_types->types[i].high, sizeof(double), 1,  file);
            fwrite(&inf_types->types[i].price_item, sizeof(double), 1,  file);
            fwrite(&inf_types->types[i].count_pairs, sizeof(int), 1,  file);
            fwrite(&inf_types->types[i].capacity_pairs, sizeof(int), 1,  file);
            if (inf_types->types[i].count_pairs>0){
                fwrite(inf_types->types[i].id_pair_domin, sizeof(int), inf_types->types[i].count_pairs, file);
            }
        }
    }
    fclose(file);
    return 0;

}

int save_pairs(const char* fname, info_pairs* inf_pairs){
    FILE* file = fopen(fname, "wb");
    if (!file){
        printf("Ошибка сохранения данных о совместимостях в файл\n");
        return 1;
    }
    int count_life_pairs = f_count_life_pairs(inf_pairs);
    fwrite(&count_life_pairs, sizeof(int), 1, file);
    for (int i = 0; i<inf_pairs->count_pairs; i++){
        if (inf_pairs->pairs[i].is_deleted == 0){
            fwrite(&inf_pairs->pairs[i].id_pair, sizeof(int), 1,  file);
            fwrite(&inf_pairs->pairs[i].code_dominant, sizeof(int), 1,  file);
            fwrite(&inf_pairs->pairs[i].code_suitable, sizeof(int), 1,  file);
            fwrite(&inf_pairs->pairs[i].count_suitable, sizeof(double), 1,  file);
            fwrite(&inf_pairs->pairs[i].sum_cost, sizeof(double), 1,  file);
            fwrite(&inf_pairs->pairs[i].id_opposite_pair, sizeof(int), 1,  file);
        }
    }
    fclose(file);
    return 0;
}