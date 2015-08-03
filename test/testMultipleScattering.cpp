#include  "MultipleScattering.h"


#include <iostream>
int main() {

  const float x0Si = MultipleScattering::x0(50,25);

  std::cout << x0Si << std::endl;
  std::cout <<   MultipleScattering::theta0(0.1f, 1.f, 1.f, 1.f) 
	    << " " <<  MultipleScattering::thetaA(1.f,0.f,  MultipleScattering::factA(0.1f))
	    << std::endl;

  std::cout <<   MultipleScattering::theta0(0.1f/cos(0.7), 1.f, 1.f/cos(0.7), 1.f) 
	    << " " <<  MultipleScattering::thetaA(1.f,tan(0.7),  MultipleScattering::factA(0.1f))
	    << std::endl;

  std::cout <<   MultipleScattering::theta0(0.1f/cos(0.2), 1.f, 1.f/cos(0.2), 1.f) 
	    << " " <<  MultipleScattering::thetaA(1.f,tan(0.2),  MultipleScattering::factA(0.1f))
	    << std::endl;


};
