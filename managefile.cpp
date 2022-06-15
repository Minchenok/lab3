#include "managefile.h"
#include "memory.h"


#include <QDebug>
FILE* openFile(char* fileName){ // Открытие файла
    return fopen(fileName, "r");
}


void scanFile(FILE* file, Data* data){ // Получение данных из файла
    rewind(file);
    char* line = allocateString(SIZE_LINE);
    char** splitLine = allocateMas(data->columCount, SIZE_LINE);
    fgets(line, SIZE_LINE, file);
    stringSplit(data->columName, line, data->columCount);
    int i = 0;
    while (fgets(line, SIZE_LINE, file) != NULL){
        if (!checkLine(line))
            continue;
        stringSplit(splitLine, line, data->columCount);
        if (!strcmp(splitLine[1], data->region)){ // Проверка региона
            copyMas(data->content[i], splitLine, data->columCount);
            i++;
        }
    }
    clearString(line);
    clearMas(splitLine, data->columCount);
}


int getLenRegion(FILE* file, Data* data){ // Получение количества строк в файле
    rewind(file);
    char* line = allocateString(SIZE_LINE);
    char** splitLine = allocateMas(data->columCount, SIZE_LINE);
    int count = 0;
    while (fgets(line, SIZE_LINE, file) != NULL){
        if (!checkLine(line))
            continue;
        stringSplit(splitLine, line, data->columCount);
        if (!strcmp(splitLine[1], data->region))
            count++;
    }
    clearString(line);
    clearMas(splitLine, data->columCount);
    return count;
}


int getCountColumn(FILE* file){ // Получение количества колонок в файле
    rewind(file);
    int count = 1;
    char* line = allocateString(SIZE_LINE);
    fgets(line, SIZE_LINE, file);
    for (int i = 0; line[i]; i++){
        if (line[i] == ','){
            count++;
        }
    }
    clearString(line);
    return count;
}

bool checkCorrectCsv(FILE* file, int countColumn){ // Проверка корректности файла
    rewind(file);
    bool error = false;
    char* line = allocateString(SIZE_LINE);
    if (line == NULL) {
        error = true;
        return error;
    }
    while (fgets(line, SIZE_LINE, file) != NULL){
        int count = 1;
        for (int i = 0; line[i]; i++){
            if (line[i] == ','){
                count++;
            }
        }
        if (count != countColumn)
            error = true;
    }
    clearString(line);
    return error;
}


bool completeData(FILE* file, Data* data){
     data->columCount = getCountColumn(file);
     data->size = getLenRegion(file, data);

     // Выделение памяти
     data->content = allocateMatrix(data->size, data->columCount, SIZE_LINE);
     data->columName = allocateMas(data->columCount, SIZE_LINE);
     if (data->content == NULL || data->columName == NULL) {
        if (data->content != NULL) {
            clearMatrix(data->content);
        }
        if (data->columName != NULL) {
            clearMas(getCountColumn, data->columName);
        }
        return false;
     } else {
        return true;
     }
}
