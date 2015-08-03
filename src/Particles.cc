#include "Particles.h"
#include "Arena.h"


Particles newParticles(Arena & arena, int N) {

  Particles p;
  p.m_size = N;

  p.m_x = arena.getf(N);
  p.m_y = arena.getf(N);
  p.m_z = arena.getf(N);

  p.m_phi = arena.getf(N);
  p.m_eta = arena.getf(N);
  p.m_pt = arena.getf(N);
  p.m_tanLambda = arena.getf(N);
  return p;
}

Particles newParticles(float * arena, int N) {

  Particles p;
  p.m_size = N;

  p.m_x = arena;
  p.m_y = arena+=N;
  p.m_z = arena+=N;

  p.m_phi = arena+=N;
  p.m_eta = arena+=N;
  p.m_pt = arena+=N;
  p.m_tanLambda = arena+=N;
  return p;
}
