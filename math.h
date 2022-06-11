#ifndef MATH_H
#define MATH_H

#include "managefile.h"


struct Numbers {
double min;
double max;
double mediana;
int minYear;
int maxYear;
double newMin;
};

void sortMas(double* mas, int size);
void setYear(Data* data, Numbers* numbers);
void swapBubls(double* a, double* b);
double getMediana(double* mas, int size);

#endif // MATH_H
