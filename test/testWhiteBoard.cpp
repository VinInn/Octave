#include "WhiteBoard.h"
#include<vector>
#include<tuple>

#include<iostream>
int main() {

  WhiteBoard event(10000);


  for (int ie=0; ie!=10; ++ie) {
    int ivl = event.put(std::vector<long long>(100));
    int ivf = event.put(std::vector<float>(100));
    int itp = event.put(std::tuple<int,float,float>(ie,1,2));

    auto vl = event.get<std::vector<long long>>(ivl);
    auto vf = event.get<std::vector<float>>(ivf);
    auto tp =  event.get<std::tuple<int,float,float>>(itp);
    
    std::cout << std::get<0>(tp) << " " <<  std::get<1>(tp) << " " << std::get<2>(tp) << std::endl;

    event.wipe();
  }


}
