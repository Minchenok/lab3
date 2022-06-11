#include "math.h"

void sortMas(double* mas, int size){ // Сортировка массива
    for (int i = 0; i < size - 1; i++){
        for (int j = i; j < size; j++){
            if (mas[i] > mas[j]){
                swapBubls(mas + i, mas + j);
            }
        }
    }
}


void swapBubls(double* a, double* b){ // Смена элементов в массиве
    double time = *a;
    *a = *b;
    *b = time;
}


double getMediana(double* mas, int size){ // Подсчет медианы
    if (size % 2){
        return mas[size/2];
    }
    else {
        return (mas[size/2] + mas[size/2 - 1]) / 2;
    }
}


void setYear(Data* data, Numbers* numbers){ // Поиск минимального и максимального года
    int minYear = atoi(data->content[0][0]);
    int maxYear = atoi(data->content[0][0]);

    for (int i = 0; i < data->size; i++){
        int time = atoi(data->content[i][0]);
        if (time < minYear)
            minYear = time;
        if (time > maxYear)
            maxYear = time;
    }
    numbers->maxYear = maxYear;
    numbers->minYear = minYear;
}
