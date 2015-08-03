#ifndef Arena_H
#define Arena_H
#include<vector>

/* A chunk of memory....
 */
class Arena {
  static constexpr int CACHE_LINE_SIZE = 64; // 64 byte cache line on x86 and x86-64
public:
  //size in 32-bit word aligned to cache lines
  Arena(int isize) : m_arena(64*((isize*4)/64+1)/4), m_size(0){
  }

  void wipe() { m_size=0;}

  void * head() { return &m_arena.front(); }

  void * get(int n=1) {
    int l = m_size;
    m_size+=n;
    if (m_size>int(m_arena.size())) throw std::bad_alloc();
    return  &(m_arena[l]);
  }
  
  float * getf(int n=1) {
    return (float*)(get(n));
  }

  int * geti(int n=1) { 
    return (int *)(get(n));
  }


  union u {
    unsigned int ui;
    int i;
    float f;
  };

  std::vector<u> m_arena;
  int m_size;

};


#endif // Arena_H
