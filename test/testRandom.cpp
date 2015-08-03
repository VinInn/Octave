#include <cmath>

#include<random>
#include<iostream>

#include "sfmt.h"



#include "cephes.h"


float __attribute__ ((aligned(16))) ai[1024];
float __attribute__ ((aligned(16))) ao[1024];

void fake() {
  for (int i=0; i!=1024; i+=2) {
   float r1 = ai[i], r2=ai[i+1];
   float u = std::sqrt(r1);
   float v = 1.f/r2;
   ao[i] = u*v; ao[i+1] = u*r2;
  }
}

void gauss(int N) {
  constexpr float pi2f=2*3.141592653589793238;

  for (int i=0; i!=N; i+=2) {
    float r1 = ai[i], r2=ai[i+1];
    float u = std::sqrt(-2.f*cephes::logf(r1));
    float c,s; cephes::sincosf(pi2f*r2,c,s);
    ao[i] = u*c; ao[i+1] = u*s;
  }
}

void gen() {
  static std::mt19937 eng;
  static std::poisson_distribution<int> poiss(100.);
  int N =  poiss(eng);
  float __attribute__ ((aligned(16))) a[1024];
  float __attribute__ ((aligned(16))) b[1024];
  float __attribute__ ((aligned(16))) g[1024];
  int outLoop = N/CRandomSFMT::arraySize;
  int trail = N -  outLoop*CRandomSFMT::arraySize;
  std::cout << outLoop << " " << trail << std::endl;

  static CRandomSFMT0 rf(1234);
  int ka=0;
  int kb=0;
  auto as = [&a, &ka](float r) { a[ka++]= 1.5f*r;};
  auto bs = [&b, &kb](float r) { b[kb++]= 3.14f*(r+1.f);};

  rf.loop(N,as);
  rf.loop(N,bs);

    
  // gauss
  constexpr float pif=3.141592653589793238;

  int kg=0;
  auto gg = [&g,&kg](float r1,float r2) {
    float u = std::sqrt(-2.f*cephes::logf(::fabs(r1)));
    float c,s; cephes::sincosf(pif*(r2+1.f),c,s);
    g[kg++] = u*c; g[kg++] = u*s;
  };
  rf.loop2((N+1)/2,gg);


  for (int i=0; i!=N; ++i) std::cout << a[i] << "," <<  b[i] << "," <<  g[i] << " " ;
  std::cout << std::endl;



}

int main(int argc, char * argv[])  {

  for (int i=0; i!=argc; ++i) gen();

  return 0;
}
