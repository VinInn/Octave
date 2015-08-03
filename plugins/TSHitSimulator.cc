#include "Producer.h"
#include "WhiteBoard.h"
#include "Config_TSHitSimulator.h"

#include "Geometry.h"

#include "Particles.h"

#include "TSimHits.h"

#include "cephes.h"

#include<cassert>
class TSHitSimulator : public Producer {
public:
  typedef Config_TSHitSimulator Configuration;
  explicit  TSHitSimulator(Configuration const&) {}

  int produce(WhiteBoard & event);

  void oneStep(Particles const & particles, int const *  __restrict__ octants, int il, int & k, TSimHits & hits);


};

int TSHitSimulator::produce(WhiteBoard & event) {
  using namespace GeometryConstants;
  // we know is 0
  auto const & oriparticles = event.get<Particles>(0);
  
  // one hit in each layer for each particle
  auto hits =  newTSimHits(event.arena(),NLayers*oriparticles.m_size);

  // as we neex to change frame and apply MS we have to copy original particle in a "workspace"
  // at this point one may envisage different technique to avoid memory explosion
  // we simply clone the whole set
  
  // get the workspace
  auto workspace = event.workspace();
  auto particles = newParticles(workspace,oriparticles.m_size);
  // we know that the memory is allocated contiguosly....
  // FIXME to be move in particle  
  ::memcpy(particles.m_x, oriparticles.m_x, sizeof(float)*7*oriparticles.m_size);
  auto octants = workspace.arena().geti(oriparticles.m_size);
  // now rotate in the octant where phi is
  // GCC does not like induction variables in memory...
  int loops = particles.m_size;
  // #pragma ivdep
  for (int i=0; i!=loops; ++i) {
    float r,d; int oct;
    particles.m_phi[i] = Geometry::rotate(particles.m_x[i],particles.m_y[i], particles.m_phi[i], 
					  r, d, oct);
    particles.m_x[i] =d;
    particles.m_y[i]=r;
    octants[i]=oct;
  }

  int k=0;
  for (int il=0; il!=NLayers; ++il) {
    oneStep(particles, octants, il, k, hits);
  }
  assert(k==hits.m_size);
  // sort? // unique??
  return event.put(std::move(hits));
  
}

void  TSHitSimulator::oneStep(Particles const & particles, int const * __restrict__ octants, int il, int & k, TSimHits & hits) {
  using namespace GeometryConstants;
  int lk=k;
  auto kernel = [&lk, &particles,  &octants, il, &hits](int ip) {
      float d,z; 
      Geometry::cross0(particles.m_phi[ip],particles.m_tanLambda[ip],radii[il]-particles.m_y[ip],d,z);
      // update the particles in the workspace
      // here in partcile x is d, y is r 
      particles.m_x[ip]+=d;
      particles.m_y[ip]=radii[il];
      particles.m_z[ip]+=z;
      d=particles.m_x[ip];
      z=particles.m_z[ip];
      if (particles.m_phi[ip]<0.f) d = sides[il]+d;  // d was negative here
      hits.m_x[lk] = int(d/xpitch[il]);
      hits.m_z[lk] = int(z/xpitch[il]);
      hits.m_id[lk] =  octants[ip]  + (il<<3);
      lk++;
  };

  int loops = particles.m_size;
  // #pragma ivdep
  for (int i=0; i!=loops; ++i) {
    kernel(i);
  }
  k=lk;

  /*
  constexpr int stride = 16;
  int loops = particles.m_size/stride;
  int tail =  particles.m_size - stride*loops;
  for (int l=0; l!=loops; ++l) {
    int is=stride*l;
    for (int i=0; i!=stride; ++i) {
      kernel(is+i);
    }
  }
  int is=stride*loops;
  for (int i=0; i!=tail; ++i) {
    kernel(is+i);
  }
  k=lk;
  */
}



PRODUCER_PLUGIN(TSHitSimulator);
