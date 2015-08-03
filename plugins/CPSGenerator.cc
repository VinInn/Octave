#include "Producer.h"
#include "WhiteBoard.h"
#include "Config_CPSGenerator.h"

#include<random>
#include<iostream>

#include "sfmt.h"

#include "Particles.h"
#include "cephes.h"

/* a Cylindrical Phase Space Generator

 */
class CPSGenerator : public Producer {
public:
  typedef Config_CPSGenerator Configuration;
  // the average number of particle
  explicit CPSGenerator(Configuration const & config) : poiss(config.aveN), ufr(1234)  {}

  int produce(WhiteBoard & event);

  // poisson generator (we use std)
  std::mt19937 eng;
  std::poisson_distribution<int> poiss;
  // the fast marsaglia modified to support vectorization  (generate uniform between -1 and 1)
  CRandomSFMT0 ufr;

};

int  CPSGenerator::produce(WhiteBoard & event) {
  int N =  poiss(eng);
  Particles p=newParticles(event.arena(),N);
  for (int i=0;i!=N;++i) {
    p.m_x[i]= p.m_y[i]= p.m_z[i]=0.f; 
  }
  int kp=0;
  int ke=0;
  int kt=0;
  auto as = [&p, &ke](float r) { p.m_eta[ke++]= 1.5f*r;};
  auto bs = [&p, &kp](float r) { p.m_phi[kp++]= 3.14f*r;};
  // pt following the famous UA1 fit
  //auto cs = [&p, &kt](float r) { p.m_pt[kt++]= 1.32f*(std::pow(std::abs(r),-0.125f)-1.f);};
  // faster??? (yes!)
  auto cs = [&p, &kt](float r) { p.m_pt[kt++]= 1.32f*(1.f/std::sqrt(std::sqrt(std::sqrt(std::abs(r))))-1.f);};

  ufr.loop(N,as);
  ufr.loop(N,bs);
  ufr.loop(N,cs);

    
  for (int i=0; i!=N; ++i) p.m_tanLambda[i] = cephes::sinhf(p.m_eta[i]);

  return event.put(std::move(p));
}

PRODUCER_PLUGIN(CPSGenerator);
