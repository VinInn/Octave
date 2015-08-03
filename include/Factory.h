#ifndef Factory_H
#define Factory_H
#include<memory>
#include<algorithm>
#include<unordered_map>
#include "Visibility.h"
#include "stringless.h"
#include <cstring>


template<typename B>
class Factory {
public:
  typedef stringless::Key Key;
  typedef std::shared_ptr<B> pointer;
  
  
  // typically instantiated statically in a library
  // name better be the name of the product of the derived class....
  Factory(Key name) {
    registry()[name]= this;
  }
  
  virtual pointer operator()(void const * conf)=0;
  
  template<typename T>
  pointer create(void const * conf) {
    typedef typename T::Configuration Configuration; 
    return pointer(new T(*(Configuration const*)conf));
  }
    
  /*
    template<typename T, typename... Args>
    pointer create(Args&&... args) {
    return pointer(new T(std::forward<Args>(args)...));
    }
  */
  
  // the registry  (it has to be global...)
  typedef  std::unordered_map<Key,Factory<B> * > Registry;
  static Registry & registry() dso_export {
    static Registry local;
    return local;
  }
  
  static pointer getOne(Key name,void const * conf) {
    auto f = registry().find(name);
    if (f==registry().end()) return pointer();
    return (*(*f).second)(conf);
  }
  
};



#endif
