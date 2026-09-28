#include "headings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void view_one_type (type one_type){
    printf("-----------------------\n");
    printf("код:            %d\n", one_type.code_type);
    printf("название:       %s\n", one_type.name);
    printf("высота:         %.3f\n", one_type.high);
    printf("стоим.1шт.:     %.3f\n", one_type.price_item);
    printf("кол-во связей:  %d\n", one_type.count_pairs);
    printf("-----------------------\n");
}

void view_types(info_types* inf_types){
    int count_types = inf_types->count_types;
    if (count_types == 0){
        printf("Список типов пустой\n");
        return;
    }
    for (int i = 0; i<count_types; i++){
        if (inf_types->types[i].is_deleted == 0){
            view_one_type (inf_types->types[i]);
        }
    }
}

void view_one_pair (pair one_pair){
    printf("----------\n");
    printf("код пары:                  %d\n",one_pair.id_pair);
    printf("код доминанта:             %d\n", one_pair.code_dominant);
    printf("код совместимого:          %d\n", one_pair.code_suitable);
    printf("оптимальное количество:    %.3f\n", one_pair.count_suitable);
    printf("суммарная стоимость:       %.3f\n", one_pair.sum_cost);
    printf("код противоположной пары:  %d\n", one_pair.id_opposite_pair);
    printf("----------\n");
}

void view_pairs(info_pairs* inf_pairs){
    int count_pairs = inf_pairs->count_pairs;
    if (count_pairs == 0){
        printf("Список совместимостей пустой\n");
        return;
    }
    for (int i = 0; i<count_pairs; i++){
        if (inf_pairs->pairs[i].is_deleted == 0){
            view_one_pair (inf_pairs->pairs[i]);
        }
    }

}