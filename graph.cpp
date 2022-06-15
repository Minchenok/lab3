#include "graph.h"

void buildAxis(QPainter* paint, GraphInfo* graphInfo, Numbers* numbers){ // Отрисовка осей
    double timeMax = (numbers -> max > 0)? numbers-> max :0; //  максимального значания
    double timeMin = (numbers -> min > 0)? numbers -> min :0;// минимум
    graphInfo -> axisX = graphInfo -> widthWindow - 2*(graphInfo->indent);
    graphInfo -> axisY = graphInfo -> heightWindow - 2*(graphInfo->indent);//отступы

    graphInfo -> kafX = (graphInfo -> axisX) /((numbers -> maxYear) - (numbers -> minYear));
    graphInfo -> kafY = (graphInfo -> axisY) /(timeMax - (numbers -> min)); //размер единичного значения
    int plusNumbersY = graphInfo -> kafY*(timeMax - timeMin); // масштаб по у
    numbers->newMin = timeMin;

    paint -> translate(graphInfo -> indent,graphInfo -> indent); // перевод короординаты
    paint->setPen(QPen(QColor(0,0,0), 2)); // цвет
    paint -> drawLine(0,0,0,graphInfo -> axisY);
    paint -> drawText(0.5 * graphInfo -> indent, 0, "Value");
    paint -> translate(0, plusNumbersY);
    paint->setPen(QPen(QColor(0,0,0), 2));
    paint -> drawLine(0,0,graphInfo -> axisX,0);
    paint -> drawText(graphInfo -> axisX - graphInfo -> indent, 0.5 * graphInfo -> indent, "Year");
}


void paintExtram(QPainter* paint, GraphInfo* graphInfo, SendData* sendData, Numbers* numbers, int size){ // Отрисовка мин и макс
    bool flagMax = false, flagMin = false;
    for (int i = 0; i < size && !flagMin; i++){
        if ((sendData + i)->value + (numbers->newMin) == numbers->max){
            paint->setPen(QPen(QColor(220, 0, 0), 7));
            double X = ((sendData + i)->year) * graphInfo->kafX;
            double Y = (sendData + i)->value * graphInfo->kafY;
            QPoint point(X, -1 * Y);
            QString name = "Max (" + QString::number((sendData + i)->year + numbers->minYear) + ";" + QString::number(numbers->max) + ")";
            paint->drawText(X + -0.5 * graphInfo->indent, -1 * Y + 0.5 * graphInfo->indent, name);
            paint->drawPoint(point);
            paint->setPen(QPen(QColor(100,100,100), 2));
            flagMin = true;
            }
        }

    for (int i = 0; i < size && !flagMax; i++){
        if ((sendData + i)->value + (numbers->newMin) == numbers->min){
            paint->setPen(QPen(QColor(220, 0, 0), 7));
            double X = ((sendData + i)->year) * graphInfo->kafX;
            double Y = (sendData + i)->value * graphInfo->kafY;
            QPoint point(X, -1 * Y);
            QString name = "Min (" + QString::number((sendData + i)->year + numbers->minYear) + ";" + QString::number(numbers->min) + ")";
            paint->drawText(X + -0.5 * graphInfo->indent, -1 * Y + 0.5 * graphInfo->indent, name);
            paint->drawPoint(point);
            paint->setPen(QPen(QColor(100,100,100), 2));
            flagMax = true;
            }
        }
}


void addDataToGraph(QPainter* paint, GraphInfo* graphInfo, SendData* sendData, Numbers* numbers, int size){ // Отрисовка данных
    paint->setPen(QPen(QColor(100,100,100), 2));
    for (int i = 1; i < size; i++){
        QPoint point1(((sendData + i - 1)->year) * graphInfo->kafX, -1 * (sendData + i - 1)->value * graphInfo->kafY);
        QPoint point2(((sendData + i)->year) * graphInfo->kafX, -1 * (sendData + i)->value * graphInfo->kafY);
        paint->drawLine(point1, point2);
    }
    paintExtram(paint, graphInfo, sendData, numbers, size);

}


void setSendData(Data* data, SendData* sendData, Numbers* numbers){ // Заполнение данных
    for (int i = 0; i < data->size; i++){
        if (strcmp(data->content[i][data->col], "-")){
            (sendData + i)->value = atof(data->content[i][data->col]) - numbers->newMin;
            (sendData + i)->year = atoi(data->content[i][0]) - numbers->minYear;
        }
    }
}


void paintValue(QPainter* paint, GraphInfo* graphInfo, Numbers* numbers){ // Отрисовка значений
    for (float i = numbers->min; i <= numbers->max; i+=0.5){
        double Y = -1 * (i - numbers->newMin) * graphInfo->kafY;
        paint->drawText(-0.8 * graphInfo->indent, Y, QString::number(i, 'g', 4));
        paint->drawPoint(0, Y);
        QPoint point1(0, Y);
        QPoint point2(graphInfo->axisX, Y);
        QPen style(QColor(100,100,100), 1);
        style.setStyle(Qt::DotLine);
        paint->setPen(style);
        paint->drawLine(point1, point2);
    }
}


void paintMedian(QPainter* paint, GraphInfo* graphInfo, double value){ // Отрисовка медианы
    paint->setPen(QPen(QColor(220,0,0), 2));
    QPoint point1(0, -1 * value * graphInfo->kafY);
    QPoint point2(graphInfo->axisX, -1 * value * graphInfo->kafY);
    paint->drawLine(point1, point2);
    paint->drawText(graphInfo->axisX - 2 * graphInfo->indent, -1 * (value * graphInfo->kafY) + graphInfo->indent, "Mediana");
    paint->setPen(QPen(QColor(100,100,100), 2));
}
