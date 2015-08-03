namespace hash64_details {
  typedef  unsigned long  long ub8;   /* unsigned 8-byte quantities */
  typedef  unsigned long  int  ub4;   /* unsigned 4-byte quantities */

  struct w { 
    constexpr w(ub8 ia,  ub8 ib,  ub8 ic) : a(ia), b(ib), c(ic){}  
    constexpr w(w iw,  ub8 ic) : a(iw.a), b(iw.b), c(iw.c+ic){}
    ub8 a;  ub8 b;  ub8 c;
  };

  constexpr w mix0(w iw) { return w((iw.a -iw.b -iw.c )^(iw.c>>43),iw.b,iw.c);}			
  constexpr w mix1(w iw) { return w(iw.a,(iw.b -iw.c -iw.a )^(iw.a<<9),iw.c);} 
  constexpr w mix2(w iw) { return w(iw.a,iw.b,(iw.c -iw.a -iw.b )^(iw.b>>8));} 
  constexpr w mix3(w iw) { return w((iw.a -iw.b -iw.c )^(iw.c>>38),iw.b,iw.c);} 
  constexpr w mix4(w iw) { return w(iw.a,(iw.b -iw.c -iw.a )^(iw.a<<23),iw.c);} 
  constexpr w mix5(w iw) { return w(iw.a,iw.b,(iw.c -iw.a -iw.b )^(iw.b>>5));} 
  constexpr w mix6(w iw) { return w((iw.a -iw.b -iw.c )^(iw.c>>35),iw.b,iw.c);} 
  constexpr w mix7(w iw) { return w(iw.a,(iw.b -iw.c -iw.a )^(iw.a<<49),iw.c);} 
  constexpr w mix8(w iw) { return w(iw.a,iw.b,(iw.c -iw.a -iw.b )^(iw.b>>11));} 
  constexpr w mix9(w iw) { return w((iw.a -iw.b -iw.c )^(iw.c>>12),iw.b,iw.c);} 
  constexpr w mix10(w iw) { return w(iw.a,(iw.b -iw.c -iw.a )^(iw.a<<18),iw.c);} 
  constexpr w mix11(w iw) { return w(iw.a,iw.b,(iw.c -iw.a -iw.b )^(iw.b>>22));}

  constexpr w mix(w iw) { return mix11(mix10(mix9(mix8(mix7(mix6(mix5(mix4(mix3(mix2(mix1(mix0(iw)))))))))))); }


  constexpr w add0(char const * k, w iw) {
    return  w(iw.a + (k[0]        +((ub8)k[ 1]<< 8)+((ub8)k[ 2]<<16)+((ub8)k[ 3]<<24)
		      +((ub8)k[4 ]<<32)+((ub8)k[ 5]<<40)+((ub8)k[ 6]<<48)+((ub8)k[ 7]<<56)),
	      iw.b + (k[8]        +((ub8)k[ 9]<< 8)+((ub8)k[10]<<16)+((ub8)k[11]<<24)
		      +((ub8)k[12]<<32)+((ub8)k[13]<<40)+((ub8)k[14]<<48)+((ub8)k[15]<<56)),
	      iw.c + (k[16]       +((ub8)k[17]<< 8)+((ub8)k[18]<<16)+((ub8)k[19]<<24)
		      +((ub8)k[20]<<32)+((ub8)k[21]<<40)+((ub8)k[22]<<48)+((ub8)k[23]<<56))
	      );
  }


  constexpr w less24(char const * k, ub4 len, w iw, ub4 oril) {
   return  w( iw.a 
	      + (len<1 ? 0 : (ub8)k[0] )
	      + (len<2 ? 0 :((ub8)k[ 1]<< 8))
	      + (len<3 ? 0 :((ub8)k[ 2]<<16))
	      + (len<4 ? 0 :((ub8)k[ 3]<<24))
	      + (len<5 ? 0 :((ub8)k[4 ]<<32))
	      + (len<6 ? 0 :((ub8)k[ 5]<<40))
	      + (len<7 ? 0 :((ub8)k[ 6]<<48))
	      + (len<8 ? 0 :((ub8)k[ 7]<<56))
	      ,
	      iw.b 
	      + (len<9 ?  0 :  (ub8)k[8]        )
	      + (len<10 ? 0 : ((ub8)k[ 9]<< 8))
	      + (len<11 ? 0 : ((ub8)k[10]<<16))
	      + (len<12 ? 0 : ((ub8)k[11]<<24))
	      + (len<13 ? 0 : ((ub8)k[12]<<32))
	      + (len<14 ? 0 : ((ub8)k[13]<<40))
	      + (len<15 ? 0 : ((ub8)k[14]<<48))
	      + (len<16 ? 0 : ((ub8)k[15]<<56))
	      ,
	      iw.c + oril
	      /* the first byte of c is reserved for the length */
	      + (len<17 ? 0 : ((ub8)k[16]<< 8))
	      + (len<18 ? 0 : ((ub8)k[17]<<16))
	      + (len<19 ? 0 : ((ub8)k[18]<<24))
	      + (len<20 ? 0 : ((ub8)k[19]<<32))
	      + (len<21 ? 0 : ((ub8)k[20]<<40))
	      + (len<22 ? 0 : ((ub8)k[21]<<48))
	      + (len<23 ? 0 : ((ub8)k[22]<<56))
	      );
  }

  constexpr w add(char const * k, ub4 len, w iw, ub4 oril) {
    return (len>=24) ? add(k+24, len-24, mix(add0(k,iw)),oril ) : mix(less24(k, len, iw, oril));
  }

}

constexpr hash64_details::ub8 hash64(char const * k, hash64_details::ub4 len, hash64_details::ub8 i) {
  return hash64_details::add(k,len,hash64_details::w(i,i,0x9e3779b97f4a7c13LL),len).c;
}



#include <cstring>
#include<vector>
#include<algorithm>

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


  struct DictElem {
    constexpr DictElem(const char * in) : m_key(hash(in)), name(in){}
    constexpr DictElem(Key ik, const char * in) : m_key(ik),name(in){}
    constexpr DictElem(Key ik=0) : m_key(ik),name(nullptr){}
    constexpr Key key() const { return m_key; }
    Key m_key;
    const char * name;
  };
  inline std::ostream & operator<<(std::ostream & co, DictElem const & elem) { return elem.name ?(co << elem.name) :  (co << "HASH_" << elem.key());}

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


  // the master dictionary;
  inline Dictionary<DictElem> & dictionary() {
    static Dictionary<DictElem> local;
    return local;
  }

}



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

    std::cout<< *stringless::dictionary().find( Instantiator<47>::kName) << std::endl;
    
    for (int i=0; i!=sizeof(Item)/sizeof(Item); ++i) {
      auto p =  stringless::dictionary().find(sequence[i].key());
      std::cout << sequence[i].key() << ": " << p << " " << ( (p!=nullptr) ? (*p) : "not found") << std::endl;
    }

  }

}


#include<iostream>
int main() {

  stringhy::runtest();

  return 0;
}
