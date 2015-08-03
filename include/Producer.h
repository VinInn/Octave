#ifndef Producer_H
#define Producer_H

class WhiteBoard;
class Producer {
public:
  virtual ~Producer(){}

  virtual int produce(WhiteBoard & event)=0;

};

#include "Factory.h"

template<typename T>
struct ProducerFactory : public Factory<Producer> {
  ProducerFactory(Key name) : Factory<Producer>(name) {}
  pointer operator()(void const * conf) {
    return create<T>(conf);
  } 
};


#include "macro_helpers.h"
#define PRODUCER_PLUGIN(type) \
  namespace {namespace CONCATENATE(plugins, __LINE__) { static constexpr stringless::Key key = stringless::hash(#type); static  ProducerFactory<type> fact(key);}}
#endif


