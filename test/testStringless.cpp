#include "stringless.h"

#include "macro_helpers.h"
#include<iostream>



namespace stringhy {

  constexpr const char * names[] = {
    "foo","bar","cross","veryveryveryverylonglonglong"
  };

  inline constexpr const char * genName(int n) { return (n==0) ? "0" : names[(n-1)%(sizeof(names)/sizeof(const char *))];}
 
  template<int N>
  struct Instantiator : public Instantiator<N-1> {
    static constexpr stringless::Key kName =  stringless::hash(genName(N));
    Instantiator() {
      stringless::dictionary().add(kName,genName(N));
    }
  };
  
  template<>
  struct Instantiator<0> {
  };
  
    
  static  Instantiator<100> instances;

  
  typedef stringless::DictElem Item;
  
  constexpr Item sequence[] = { "foo", "cross", "notThere",  "cross"};
  

  void runtest() {

    stringless::dictionary().dump(std::cout);

    std::cout<< *stringless::dictionary().find( Instantiator<46>::kName) << std::endl;
    
    for (int i=0; i!=sizeof(Item)/sizeof(Item); ++i) {
      auto p =  stringless::dictionary().find(sequence[i].key());
      std::cout << sequence[i].key() << ": " << p << " " << ( (p!=nullptr) ? (*p) : "not found") << std::endl;
    }

  }

}

#define STRINGLESS
namespace notstringhy {

#undef stringless_H
#include "stringless.h"



  constexpr const char * names[] = {
    "foo","bar","cross","veryveryveryverylonglonglong"
  };

  inline constexpr const char * genName(int n) { return (n==0) ? "0" : names[(n-1)%(sizeof(names)/sizeof(const char *))];}
 
  template<int N>
  struct Instantiator : public Instantiator<N-1> {
    static constexpr stringless::Key kName =  stringless::hash(genName(N));
    Instantiator() {
      stringless::dictionary().add(kName,genName(N));
    }
  };
  
  template<>
  struct Instantiator<0> {
  };
  
    
  static  Instantiator<100> instances;

  typedef stringless::DictElem Item;

  constexpr Item sequence[] = { "foo", "cross", "notThere",  "cross"};
  
  

  void runtest() {
    stringless::dictionary().dump(std::cout);

    std::cout<< *stringless::dictionary().find( Instantiator<46>::kName) << std::endl;
    
    for (int i=0; i!=sizeof(sequence)/sizeof(Item); ++i) {
      auto p =  stringless::dictionary().find(sequence[i].key());
      std::cout << sequence[i].key() << ": " << p << " ";
      if (p)  std::cout << (*p); else std::cout << "not found";  
      std::cout << std::endl;
    }

  }

}


#include<iostream>
int main() {

  stringhy::runtest();
  std::cout <<"\n\n\n\n" << std::endl;
  notstringhy::runtest();


  return 0;
}
