#ifndef LOGIC_H
#define LOGIC_H

#include "math.h"
#include "managefile.h"

struct SendData{
  int year;
  double value;
};


bool loadData(Data* data);
bool calcColm(Data* data, Numbers* numbers);
bool completeNumbers(Data* data, Numbers* numbers);



#endif // LOGIC_H
