#ifndef GRAPH_H
#define GRAPH_H
#define INDENT 40

#include <QPainter>
#include "math.h"
#include "managefile.h"
#include "logic.h"


struct GraphInfo{
    int widthWindow; // Ширина окна
    int heightWindow; // Высота окна
    int indent; // Отступ
    int axisY; // Высота оси OY
    int axisX; // Высота оси OX
    double kafY; // Кол-во пикселей для одной единицы измерения по оси OY
    double kafX; // Кол-во пикселей для одной единицы измерения по оси OX
};




void buildAxis(QPainter* paint, GraphInfo* graphInfo, Numbers* numbers);
void addDataToGraph(QPainter* paint, GraphInfo* graphInfo, SendData* sendData, Numbers* numbers, int size);
void setSendData(Data* data, SendData* sendData, Numbers* numbers);
void paintMedian(QPainter* paint, GraphInfo* graphInfo, double value);
void paintValue(QPainter* paint, GraphInfo* graphInfo, Numbers* numbers);

#endif // GRAPH_H
