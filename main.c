#include <stdio.h>
#include "headings.h"
#include <windows.h>
#include <locale.h> // ИСПРАВЛЕНИЕ

int main(void){
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    setlocale(LC_NUMERIC, "C"); // ИСПРАВЛЕНИЕ
    printf("ВЫБЕРИТЕ:\n");
    printf("1. ЗАГРУЗКА ДАННЫХ\n");
    printf("10. ВЫХОД\n");
    printf("***функции 2-9 недоступны до загрузки данных\n");
    int choice;
    while (1){
        if (scanf("%d", &choice) != 1 || !(choice == 1 || choice == 10) ){
            printf("Необходимо ввести 1 или 10\n");
            clear_buffer();
        }
        else
            break;
    }
    if (choice == 10){
        printf("Программа завершена!\n");
        return 0;
    }
    info_types inf_types = {NULL, 0, 0};
    if (load_types("flowers_types.bin", &inf_types) != 0){
        printf("Файл с типами не найден. Будет создан новый список.\n");
        inf_types.capacity_types = 10;
        inf_types.types = malloc(inf_types.capacity_types * sizeof(type));
        inf_types.count_types = 0;
    }
    info_pairs inf_pairs = {NULL, 0, 0};
    if (load_pairs("flowers_pairs.bin", &inf_pairs) != 0){
        printf("Файл с парами не найден. Будет создан новый список.\n");
        inf_pairs.capacity_pairs = 10;
        inf_pairs.pairs = malloc(inf_pairs.capacity_pairs * sizeof(pair));
        inf_pairs.count_pairs = 0;
    }
    while (1){
        printf("===============МЕНЮ=================\n");//
        printf("===2. Просмотр списков ===\n"); //
        printf("===3. Сортировка ===\n"); //
        printf("===4. Поиск по фильтрам ===\n"); //
        printf("===5. Добавление данных ===\n");//
        printf("===6. Удаление данных ===\n");
        printf("===7. Редактирование данных ===\n");
        printf("===8. Составление букетов ===\n");
        printf("===9. ВЫХОД (с сохранением данных) ===\n");//
        printf("===10. ВЫХОД (без сохранения данных) ===\n");//
        printf("====================================\n");        
        if (scanf("%d", &choice)!=1) {
            printf("Неверный формат ввода (необходимо число)\n");
            clear_buffer();
            continue;
        }
        switch (choice){
            case 2:{ // просмотр списков
                int choice_view = choose_spisok();
                if (choice_view == 1) {
                    view_types(&inf_types);
                } else {
                    view_pairs(&inf_pairs); 
                }

                break;
            }
            case 3:{ // сортировка
                int choice_sort = choose_spisok();
                printf("Выберите направление сортировки:\n1 по возрастанию\n-1 по убыванию\n");
                int dir;
                while (1){
                    if(scanf("%d", &dir) != 1 || !(dir == 1 || dir == -1)){
                        printf("Некорректный ввод направления\n");
                        clear_buffer();
                        continue;
                    }
                    break;
                }
                printf("==============================\n");
                printf("Выберите критерий сортировки:\n");
                int kriterij_sort;
                int (*comparator_t) (type, type, int );
                if (choice_sort == 1){
                    printf("1. Название цветка\n");
                    printf("2. Высота цветка\n");
                    printf("3. Стоимость 1 цветка\n");
                    while (1){
                    if(scanf("%d", &kriterij_sort) != 1 || !(kriterij_sort == 1 || kriterij_sort == 2|| kriterij_sort == 3)){
                        printf("Некорректный ввод критерия сортировки для списка типов цветков\n");
                        clear_buffer();
                        continue;
                        }
                        break;
                    }
                    switch (kriterij_sort){
                        case 1:{
                            comparator_t = compare_type_name;
                            break;
                        }
                        case 2:{
                            comparator_t = compare_type_height;
                            break;
                        }
                        case 3:{
                            comparator_t = compare_type_price;
                            break;
                        }
                    }
                    sort_types(&inf_types, comparator_t, dir);
                } else{
                    printf("1. Код доминирующего цветка\n");
                    printf("2. Суммарная стоимость обоих цветков\n");
                    int (*comparator_p)(pair, pair, int);
                    while (1){
                        if(scanf("%d", &kriterij_sort) != 1 || !(kriterij_sort == 1 || kriterij_sort == 2)){
                            printf("Некорректный ввод критерия сортировки для списка совместимостей цветков\n");
                            clear_buffer();
                            continue;
                        }
                        break;
                    }
                    switch (kriterij_sort){
                        case 1:{
                            comparator_p = compare_pair_dominant;
                            break;
                        }
                        case 2:{
                            comparator_p = compare_pair_sumcost;
                            break;
                        }
                    }
                    sort_pairs(&inf_pairs, comparator_p, dir);
                    
                }
                printf("==============================\n");
                break;
            }
            case 4:{ // поиск по фильтрам
                int choice_filtr = choose_spisok();
                int type_filtr;
                if (choice_filtr == 1){
                    printf("1. По коду типа цветка\n");
                    printf("2. По названию типа цветка\n");
                    printf("3. По стоимости типа цветка\n");
                    while (1){
                        if(scanf("%d", &type_filtr) != 1 || !(type_filtr == 1 || type_filtr == 2 || type_filtr == 3)){
                            printf("Некорректный ввод фильтра поиска для списка типов цветков\n");
                            clear_buffer();
                            continue;
                        }
                        break;
                    }
                    switch (type_filtr){
                        case 1:{

                            filter_type_by_code(inf_types); 
                            break; 
                        }
                        case 2:{
                            filter_type_by_name(inf_types); 
                            break; 
                        }
                        case 3:{
                            filter_type_by_price(inf_types); 
                            break;
                        }
                    }

                }else{
                    printf("1. По коду доминирующего типа цветка\n");
                    printf("2. По суммарной стоимости цветков обоих типов\n");
                    while (1){
                        if(scanf("%d", &type_filtr) != 1 || !(type_filtr == 1 || type_filtr == 2)){
                            printf("Некорректный ввод фильтра поиска для списка совместимостей цветков\n");
                            clear_buffer();
                            continue;
                        }
                        break;
                    }
                    switch (type_filtr){
                        case 1:{
                            filter_pair_by_code_domin(inf_pairs); 
                            break;
                        }
                        case 2:{
                            filter_pair_by_sum_cost(inf_pairs);   
                            break;
                        }
                        
                    }
                }
                break;
            }
            case 5:{ // добавление данных
                int choice_add = choose_spisok();
                if (choice_add == 1){
                   add_type(&inf_types); 
                }else{
                    add_pair(&inf_pairs, &inf_types);
                }
                break;
            }
            case 6:{ // удаление 
                int choice_del = choose_spisok();
                if (choice_del == 1){
                   delete_type(&inf_types);
                }else{
                    delete_pair_and_opposite(&inf_pairs, &inf_types);

                }
                break;
            }
            case 7:{ // редактирование
                int choice_edit = choose_spisok();
                if (choice_edit == 1){
                   update_types(&inf_types, &inf_pairs);
 
                }else{
                    update_pairs(&inf_types, &inf_pairs);

                }
                break;
            }
            case 8:{ // букеты
                special_fun(&inf_types, &inf_pairs);
                break;
            }
            case 9:{
                save_types("flowers_types.bin", &inf_types);
                save_pairs("flowers_pairs.bin", &inf_pairs);
                printf("Программа завершена c сохранением данных!\n");
                free_types(&inf_types);
                free_pairs(&inf_pairs);
                return 0;
            }
            case 10:{
                printf("Программа завершена без сохранения данных!\n");
                free_types(&inf_types);
                free_pairs(&inf_pairs);
                return 0;
            }
            default:
                printf("Неверный выбор!\n");
        }

    }
    return 0;
}


/* данные в бин файле хранятся так: count_types, 
потом это количесвто раз блок 
(code_type, name, high, price_item, count_pairs, 
    (числа количеством count_pairs, , то есть те самые id пар)), 
далее count_pairs, снова блок count_pairs раз 
(id_pair, code_dominant, code_suitable, count_suitable, 
    sum_cost, id_opposite_pair). при этом capacity_types и capacity_pairs 
    заполняется как count + 10%. */