/* the scheduler
 

 */
#include "Scheduler.h"
#include "Registerer.h"

#include<vector>
#include<algorithm>
namespace {

  struct ModuleDescr{
    typedef stringless::Key Key;
    constexpr ModuleDescr(Key itype, Key iname, void const * in) : type(itype), name(iname), conf(in){}
    constexpr ModuleDescr(Key ik) : type(0),name(ik),conf(nullptr){}
    constexpr Key key() const { return name; }
    
    Key type;
    Key name;
    void const * conf;
  };
  inline bool operator<(ModuleDescr const & rh,ModuleDescr const & lh) { return rh.key() < lh.key();}
  inline bool operator==(ModuleDescr const & rh,ModuleDescr const & lh) { return rh.key() == lh.key();}
  inline bool operator!=(ModuleDescr const & rh,ModuleDescr const & lh) { return rh.key() != lh.key();}
  
  
  static stringless::Dictionary<ModuleDescr> modules;
  
  typedef stringless::WKey<std::pair<stringless::DictElem const *,stringless::DictElem const *> > SequenceItem;
  
  static stringless::Dictionary<SequenceItem> sequences;
  
  
}

#include<iostream>
void  Registerer::add(Key obj, Key name, void const * conf, const char * cobj, const char * cname){
  std::cout << "registering ";
  if (cobj)  std::cout	<< cobj;
  if (cname)  std::cout << " with name " << cname;
  std::cout << std::endl; 
  stringless::dictionary().add(obj,cobj);
  stringless::dictionary().add(name,cname);
  modules.add(obj,name,conf);
}


void SequenceRegisterer::add(Key name, stringless::DictElem const * b, stringless::DictElem const * e, const char * cname) {
  std::cout << "registering sequence";
  if (cname)  std::cout << " with name " << cname;
  std::cout << " of size " << e-b << std::endl; 
  stringless::dictionary().add(name,cname);
  sequences.add(name,b,e);
}

#include "Producer.h"
#include<iostream>

namespace{
  inline void buildSequence(stringless::DictElem const * p, stringless::DictElem const * e, 
		     std::shared_ptr<Producer> * prods, stringless::DictElem * names) {
    typedef Factory<Producer> Fact;
    
    std::cout << "# factories " << Fact::registry().size() << std::endl;
    
    
    for (int i=0; p!=e; ++p, ++i) {
      stringless::dictionary().add(*p);
      auto descrp = modules.find((*p).key());
      if (!descrp) {
	std::cout << "producer " << *stringless::dictionary().find((*p).key()) << " not found???" << std::endl;
	continue;
      }
      auto producer = Fact::getOne(descrp->type,descrp->conf);
      names[i] = *stringless::dictionary().find(descrp->name);
      std::cout << "producer " << names[i] << " is a " <<  typeid(*producer).name() << std::endl;
      prods[i] = producer;
    }
    // clear memory
    stringless::Dictionary<ModuleDescr> garbage;
    std::swap(modules, garbage);
  }

}


Scheduler::Scheduler(){}

#include "WhiteBoard.h"
#include "Producer.h"
#include <vector>

#include "Stat.h"
#include <x86intrin.h>
inline volatile unsigned long long rdtsc() {
  return __rdtsc();
  /*
  volatile long long int x;
  __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
  return x;
  */
}

void Scheduler::run(WhiteBoard & event, int nev) {
  
  // for the time being just look for sequence "MainSequence"
  constexpr stringless::Key main =  stringless::hash( "MainSequence");
  auto seqp = sequences.find(main);
  if (!seqp) {
    std::cout <<  "MainSequence not found" << std::endl;
    abort();
  }

  auto const & seq = *seqp;
  size_t np = seq.value().second-seq.value().first;
  std::vector<std::shared_ptr<Producer> > producers(np);
  std::vector<stringless::DictElem> names(np);
  buildSequence(seq.value().first,seq.value().second,&producers.front(),&names.front());
  
  std::vector<Stat<double> > times(np+1);
  
  for (int ie=0; ie!=nev; ++ie) {
    if (nev<10 || ie%(nev/10)==0) std::cout << "Event " << ie << std::endl;
    auto volatile et = rdtsc();
    for (int ip=0; ip!=np; ++ip) {
      if (nev<10 || ie%(nev/10)==0) std::cout << "Producer " << names[ip] << std::endl;
      auto volatile pt = rdtsc();
      producers[ip]->produce(event);
      auto volatile pe = rdtsc() -pt;
      if(pe<10.e6) times[ip].add(pe);
    }
    event.wipe();
    auto volatile ee = rdtsc()-et;
    if(ee<10.e6) times[np].add(ee);
  }
  
  auto dumptime = [](Stat<double> const & stats) {
    std::cout << stats.n << " " << stats.ave() << " " << stats.rms()  << " " << stats.amin  << " " << stats.amax;
  };
  
  
  std::cout << "\nTimes\nEvent : "; dumptime(times[np]); std::cout << std::endl;
  for (int ip=0; ip!=np; ++ip) {
    std::cout << names[ip] <<" : ";dumptime(times[ip]); std::cout << std::endl;
  }
  
}


