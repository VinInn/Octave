#ifndef Particle_H
#define Particle_H
/* store for N particle allocated in the event
 */
class Particles {

public:

  float *  __restrict__ m_x;
  float *  __restrict__ m_y;
  float *  __restrict__ m_z;


  float *  __restrict__ m_phi;
  float *  __restrict__ m_eta;
  float *  __restrict__ m_pt;
  // a cache
  float *  __restrict__ m_tanLambda;
  // 
  int m_size;
};

class Arena;
Particles newParticles(Arena & arena, int N);
Particles newParticles(float * arena, int N);



#endif
