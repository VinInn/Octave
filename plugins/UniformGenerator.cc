#include "Producer.h"
#include "WhiteBoard.h"
#include "Config_UniformGenerator.h"

#include<random>
#include<iostream>

#include "sfmt.h"

#include "Particles.h"
#include "cephes.h"

/*  Generated N particle uniformly in eta,phi,pt

 */
class UniformGenerator : public Producer, private  Config_UniformGenerator {
public:
  typedef Config_UniformGenerator Configuration;
  // the average number of particle
  explicit UniformGenerator(Configuration const & config) :  Configuration(config), ufr(1234)  {}

  int produce(WhiteBoard & event);

  // the fast marsaglia modified to support vectorization (generate uniform between -1 and 1)
  CRandomSFMT0 ufr;
  
};

int  UniformGenerator::produce(WhiteBoard & event) {
  auto ran = [](float r, float mn, float mx) {  return mn + 0.5f*(mx-mn)*(r+1.f);};

  Particles p=newParticles(event.arena(),N);
  int kp=0;
  int ke=0;
  int kt=0;
  auto as = [&p, &ke,ran,this](float r) { p.m_eta[ke++]= ran(r,min_eta,max_eta);};
  auto bs = [&p, &kp,ran,this](float r) { p.m_phi[kp++]= ran(r,min_phi,max_phi);};
  auto cs = [&p, &kt,ran,this](float r) { p.m_pt[kt++]=ran(r,min_pt,max_pt);};
  
  ufr.loop(N,as);
  ufr.loop(N,bs);
  ufr.loop(N,cs);
  
  
  for (int i=0; i!=N; ++i) p.m_tanLambda[i] = cephes::sinhf(p.m_eta[i]);

  return event.put(std::move(p));
}

PRODUCER_PLUGIN(UniformGenerator);
