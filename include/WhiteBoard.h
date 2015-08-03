#ifndef WhiteBoard_H
#define WhiteBoard_H

#include "Arena.h"
#include <memory>

/* A collection of collections (sic)
 */
class WhiteBoard {
private:
  struct Wrapper{ virtual ~Wrapper(){}  virtual void * get()=0;}; 
  template<typename T> struct Owner : public Wrapper {
    Owner(T && iprod) : me(std::move(iprod)){}    
    void * get() { return &me;} // must verify typeinfo
    T me;
  };

public: 
  WhiteBoard(int asize, int wsize=-1) : m_arena(asize),  m_workspace(wsize>0 ? wsize : asize){}

  void wipe() { 
    m_products.clear();
    m_arena.wipe();
  }

  Arena & arena() { return m_arena;}

  // RAII
  struct Workspace {
    Workspace(WhiteBoard & iwb) : wb(iwb){}
    ~Workspace() {wb.m_workspace.wipe();}
    operator Arena & () { return wb.m_workspace;}
    Arena & arena() { return wb.m_workspace;}
    
 private:
    WhiteBoard & wb;
  };
  
  friend Workspace;
  
  Workspace workspace() { return Workspace(*this);}

  template<typename T>
  int put(T && iprod) {
    // here we shall clever allocate it...
    m_products.push_back(std::unique_ptr< Wrapper>(new Owner<T>(std::move(iprod))));
    return m_products.size()-1;
  }

  template<typename T>
  T const & get(int i) const {
    return  *(T const *)(*m_products[i]).get();
  }

private:

  std::vector<std::unique_ptr< Wrapper> > m_products;
  Arena m_arena;
  Arena m_workspace;

};


#endif // WhiteBoard_H
