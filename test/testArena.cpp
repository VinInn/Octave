#include "Arena.h"


#include<iostream>
int main() {

  Arena arena(10000);

  float * x = arena.getf(100);
  float * y = arena.getf(100);
  int * k = arena.geti(100);


  for (int i=0; i!=100; ++i) {
    k[i] = i;
    x[i] = k[i];
    y[i] = 1.f/(1.f+x[i]);
  }

  std::cout << y[99] << std::endl;

}
