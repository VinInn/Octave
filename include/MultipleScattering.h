#ifndef MultipleScattering_H
#define MultipleScattering_H
#include<cmath>
#include<cephes.h>
class MultipleScattering {
public:

  static float theta0(float x, float beta, float p, float x0) { return 0.136f*std::sqrt(x/x0)*(1.f + 0.038f*cephes::logf(x/x0))/(beta*p); }

  // approximation almost normal incidence, relativistic particle
  static float factA(float radlen) { return 0.136f*std::sqrt(radlen)*(1.f + 0.038f*std::log(radlen));}
  static float thetaA(float pt, float tl, float fact) { float cl = 1.f/std::sqrt(1.f+tl*tl); return fact*std::sqrt(cl)/pt;} 

  static /* constexpr */ float x0(float A, float Z) { return 716.4f*A/(Z*(Z+1)*logf(287.f/std::sqrt(Z))); }

};


#endif
