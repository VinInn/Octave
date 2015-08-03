// this is a configuration  file 
#include "Registerer.h"

// each can be in a different fragment and even more nested namespaces

#include "Config_CPSGenerator.h"
namespace {

  static Config_CPSGenerator c10;
  REGISTER(CPSGenerator, CPSGenerator10, c10)

  static Config_CPSGenerator c100;
  REGISTER(CPSGenerator, CPSGenerator100, c100)

  struct L1 {
    L1() {
      c10.aveN=10;
      c100.aveN=100;
    }
  };
  static L1 l1;

}

#include "Config_TSHitSimulator.h"
namespace {
  static Config_TSHitSimulator tshs;
  REGISTER(TSHitSimulator, TSHitSimulator, tshs)
  REGISTER(TSHitSimulator0, TSHitSimulator0, tshs)
}

#include "Config_PhiPattern.h"
namespace {
  static Config_PhiPattern pp20(6280);
  REGISTER(PhiPattern,PhiPattern,pp20)
}


// the sequence to run
namespace {
  SEQUENCE(MainSequence) = { "CPSGenerator100", "TSHitSimulator", "PhiPattern" };
  REGISTER_SEQUENCE(MainSequence);
}


#include "Scheduler.h"
#include "WhiteBoard.h"
#include<iostream>


int main(int a, char ** ) {
  int nev=100000;
  if(a>1) nev=5;
  WhiteBoard event(10000);
  Scheduler sched;
  sched.run(event,nev);
  
  return 0;
}

