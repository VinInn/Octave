#include "TSimHits.h"
#include "Arena.h"


TSimHits newTSimHits(Arena & arena, int N) {

  TSimHits p;
  p.m_size = N;
  p.m_id = arena.geti(N);
  p.m_x = arena.geti(N);
  p.m_z = arena.geti(N);
  return p;
}
