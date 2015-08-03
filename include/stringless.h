#ifndef stringless_H
#define stringless_H

// #define nullptr 0

#include "constHash64.h"
#include <cstring>
#include<vector>
#include<algorithm>
#include "Visibility.h"

//move out of line?
#include<ostream>

namespace stringless {
  typedef unsigned long long Key;
  static constexpr Key hash(const char * name) { return ::hash64(name,strlen(name),0);}

  template<typename T>
  struct WKey { 
    typedef T value_type;
    constexpr WKey(Key k=0) : m_key(k), m_value() {}
    constexpr WKey(const char * name) : m_key(hash(name)), m_value() {}
    constexpr WKey(Key k, T const & t) : m_key(k), m_value(t) {}
    template<typename... Args>
    constexpr WKey(Key k, Args&&... args) :  m_key(k), m_value(std::forward<Args>(args)...){}
    constexpr Key key() const { return m_key; }
    constexpr value_type const & value() const { return m_value;} 
    Key m_key;
    value_type m_value;
  };
  template<typename T>
  inline bool operator<(WKey<T> const & rh,WKey<T> const & lh) { return rh.key() < lh.key();}
  template<typename T>
  inline bool operator==(WKey<T> const & rh,WKey<T> const & lh) { return rh.key() == lh.key();}
  template<typename T>
  inline bool operator!=(WKey<T> const & rh,WKey<T> const & lh) { return rh.key() != lh.key();}


#ifdef STRINGLESS
  struct DictElem {
    constexpr DictElem(const char * in) : m_key(hash(in)) {}
    constexpr DictElem(Key ik, const char *) : m_key(ik){}
    constexpr DictElem(Key ik=0) : m_key(ik){}
    constexpr Key key() const { return m_key; }
    Key m_key;
  };
  inline std::ostream & operator<<(std::ostream & co, DictElem const & elem) { return co << "HASH_" << elem.key();}
#else
  struct DictElem {
    constexpr DictElem(const char * in) : m_key(hash(in)), name(in){}
    constexpr DictElem(Key ik, const char * in) : m_key(ik),name(in){}
    constexpr DictElem(Key ik=0) : m_key(ik),name(nullptr){}
    constexpr Key key() const { return m_key; }
    Key m_key;
    const char * name;
  };
  inline std::ostream & operator<<(std::ostream & co, DictElem const & elem) { return elem.name ?(co << elem.name) :  (co << "HASH_" << elem.key());}

 #endif 
  inline bool operator<(DictElem const & rh,DictElem const & lh) { return rh.key() < lh.key();}
  inline bool operator==(DictElem const & rh,DictElem const & lh) { return rh.key() == lh.key();}
  inline bool operator!=(DictElem const & rh,DictElem const & lh) { return rh.key() != lh.key();}
  
  template<typename T>
  class Dictionary {
  private:
    typedef T Elem;
    typedef std::vector<Elem> Dict;
    Dict dict;
    bool nosort;
  public:
    Dictionary() : nosort(false){}

   template<typename... Args>
   void add(Args&&... args) {
      // check if present???
      dict.push_back(Elem(std::forward<Args>(args)...));
      nosort=true;
    }
    
    T const * find(Key key) const {
      if (nosort) {  const_cast<Dictionary*>(this)->sort();}
      auto p = std::lower_bound(dict.begin(),dict.end(),T(key));
      if (p==dict.end() || (*p).key()!=key ) return nullptr;
      return  &(*p);
    }


   void sort() {
    std::sort(dict.begin(),dict.end()); 
    nosort=false;
   }

    void dump(std::ostream & co) const {
      if (nosort) {  const_cast<Dictionary*>(this)->sort();}
      for (auto const & e : dict) 
	co << e.key() << " " << e << std::endl;
    }

 };


  // the master dictionary
  Dictionary<DictElem> & dictionary() dso_export;
  inline Dictionary<DictElem> & dictionary() {
    static Dictionary<DictElem> local;
    return local;
  }

}

#ifdef STRINGLESS
#define STRING(x) nullptr
#else
#define STRING(x) x
#endif


#endif // stringless
