#include "logic.h"
#include "math.h"

bool loadData(Data* data){ // Работа с файлом
    bool error = false;
    FILE* file = fopen(data->path, "r");

    if (file){

        if (data->content){
            // Очистка памяти
            clearMatrix(data->content, data->size, data->columCount);
            clearMas(data->columName, data->columCount);
        }
        if (completeData(file, data)){
            if (!checkCorrectCsv(file, data->columCount)){
                scanFile(file, data);
            }
            else {
                error = true;
            }
        }
        else{
            error = true;
        }
        fclose(file);
    }
    else {
        error = true;
    }
    return error;
}


bool isColNumValid(Data* data, Numbers* numbers){ // Проверка корректности номера колонки
    bool error = false;
    if (data->col > data->columCount || data->col < 2){
        error = true;
    }
    else {
        error = completeNumbers(data, numbers);
    }
    return error;
}


bool completeNumbers(Data* data, Numbers* numbers){ // Заполнение структуры Numbers (максимальное, минимальное, медиана)
    double* masNumber = (double*)calloc(data->size, sizeof(double));

    if (masNumber == NULL || !data->size){
        return true;
    }
    for (int i = 0; i < data->size; i++){
        masNumber[i] = atof(data->content[i][data->col]);
    }
    sortMas(masNumber, data->size);
    numbers->max = masNumber[data->size - 1];
    numbers->min = masNumber[0];
    numbers->mediana = getMediana(masNumber, data->size);
    setYear(data, numbers);
    free(masNumber);
    return false;
}


