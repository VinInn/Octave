#ifndef  Registerer_H
#define  Registerer_H
#include "stringless.h"
#include<memory>

#include "Visibility.h"


struct Registerer {
  typedef stringless::Key Key;

  Registerer(const char * obj, const char * name, void * conf) {
    add(stringless::hash(obj), stringless::hash(name),conf,obj,name);
  }

  Registerer(Key obj, Key name, void * conf, const char * cobj, const char * cname) {
    add(obj, name, conf, cobj, cname);
  }

  static void add(Key obj, Key name, void const * conf, const char * cobj, const char * cname)  dso_export;

};


#include "macro_helpers.h"
#define REGISTER(type,name,config) \
  namespace  CONCATENATE(local, __LINE__) {			 \
    constexpr  stringless::Key kName =  stringless::hash(#name); \
    constexpr  stringless::Key kType =  stringless::hash(#type); \
    static Registerer  rc0(kType, kName, &config,STRING(#type),STRING(#name)); \
  }

struct SequenceRegisterer {
  typedef stringless::Key Key;

 
  SequenceRegisterer(Key name, stringless::DictElem const * b, stringless::DictElem const * e, const char * cname) {
    add(name, b, e, cname);
  }

  static void add(Key name, stringless::DictElem const * b, stringless::DictElem const * e, const char * cname)  dso_export;

};



#define SEQUENCE(name) \
  constexpr stringless::Key  CONCATENATE(name, Key) = stringless::hash(#name); \
  constexpr stringless::DictElem name[]

#define REGISTER_SEQUENCE(name) \
  static SequenceRegisterer CONCATENATE(name,Reg)(CONCATENATE(name, Key), name, name+sizeof(name)/sizeof(stringless::DictElem),STRING(#name));


#endif
