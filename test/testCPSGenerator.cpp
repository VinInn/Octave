#include "WhiteBoard.h"
#include "Factory.h"
#include "Producer.h"

#include<iostream>
#include<typeinfo>
#include "stringless.h"
#include "Config_CPSGenerator.h"


int main() {
  constexpr  stringless::Key kName =  stringless::hash("CPSGenerator");
  Config_CPSGenerator config;
  config.aveN=10;

  typedef Factory<Producer> Fact;

  std::cout << "# factories " << Fact::registry().size() << std::endl;
 
  auto producer = Fact::getOne(kName,&config);

  std::cout << "producer " << typeid(*producer).name() << std::endl;

  WhiteBoard event(10000);


  for (int ie=0; ie!=10; ++ie) {
    std::cout << "Event " << ie << std::endl;
    int prid = producer->produce(event);

    event.wipe();
  }
  return 0;
}
