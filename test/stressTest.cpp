// the trivial main
#include "Scheduler.h"
#include "WhiteBoard.h"
#include<iostream>


int main() {
  
  
  WhiteBoard event(10000);
  Scheduler sched;
  sched.run(event,10);
  
  return 0;
}
