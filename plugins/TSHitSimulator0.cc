#include "Producer.h"
#include "WhiteBoard.h"
#include "Config_TSHitSimulator.h"

#include "Geometry.h"

#include "Particles.h"

#include "TSimHits.h"

#include "cephes.h"

class TSHitSimulator0 : public Producer {
public:
  typedef Config_TSHitSimulator Configuration;
  explicit  TSHitSimulator0(Configuration const&) {}

  int produce(WhiteBoard & event);

  void oneStep(Particles const & particles, int il, int & k, TSimHits & hits);


};

int TSHitSimulator0::produce(WhiteBoard & event) {
  using namespace GeometryConstants;
  // we know is 0
  auto const & particles = event.get<Particles>(0);

  auto hits =  newTSimHits(event.arena(),NLayers*particles.m_size);
  int k=0;
  for (int il=0; il!=NLayers; ++il) {
    oneStep(particles, il, k, hits);
  }

  // sort?
  return event.put(std::move(hits));
  
}

void  TSHitSimulator0::oneStep(Particles const & particles, int il, int & k, TSimHits & hits) {
  using namespace GeometryConstants;
  int lk=k;
  auto kernel = [&lk, &particles, il, &hits](int ip) {
      float x,z; int oct;
      Geometry::cross(particles.m_x[ip],particles.m_y[ip],particles.m_z[ip],
		      particles.m_phi[ip],particles.m_tanLambda[ip],radii[il],x,z,oct);
      hits.m_x[lk] = int(x/xpitch[il]);
      hits.m_z[lk] = int(z/xpitch[il]);
      hits.m_id[lk] =  oct + (il<<3);
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



PRODUCER_PLUGIN(TSHitSimulator0);
