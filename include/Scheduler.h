#ifndef Scheduler_H
#define Scheduler_H
#include "Visibility.h"

class  WhiteBoard;

class Scheduler {
public:
  Scheduler()  dso_export;


  void run(WhiteBoard & event, int nev)  dso_export;


};
#endif
