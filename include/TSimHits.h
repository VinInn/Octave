#ifndef TSimHits_H
#define TSimHits_H
/* store for N particle allocated in the event
 */
class TSimHits {

public:

  int *  __restrict__ m_id;
  int *  __restrict__ m_x;
  int *  __restrict__ m_z;
  // 
  int m_size;
};

class Arena;
TSimHits newTSimHits(Arena & arena, int N);



#endif
