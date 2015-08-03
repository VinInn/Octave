#ifndef Geometry_H
#define Geometry_H


#include "Arena.h"
#include "cephes.h"
#include <memory>
#include "carray.h"
/*
  Octave Geometry
  12 octagonal layers (polihedra)
  Each side of a layer is composed by 4 square detectors
  radius of layers adjustable : the rest is consequent...
 */

namespace GeometryConstants {
  static constexpr int NLayers = 12;
  static constexpr float pif = std::acos(-1.);
  static constexpr float pi2f = 2*pif;
  static constexpr float pio4 = 0.25*pif;
  static constexpr float cosPIo8=0.923879532511;
  static constexpr float cosPIo4=0.707106781187;
  static constexpr float sinPIo8=0.382683432265;
  static constexpr std::array<float,NLayers>  radii = {{1.f,2.f,3.f,
						   10.f,15.f,20.f,25.f,30.f, 
						   50.f,60.f,70.f,75.f}};
  static constexpr std::array<float,NLayers>  xpitch = {{1.f/1024.f,1.f/1024.f,1.f/1024.f,
						    10.f/1024.f,10.f/1024.f,10.f/1024.f,10.f/1024.f,
						    50.f/1024.f,50.f/1024.f,50.f/1024.f,50.f/1024.f}};
  
  constexpr float side(int i) { return  (cosPIo4/cosPIo8)*radii[i];}
  static constexpr auto sides = make_carray<NLayers>(side);
  
}

class Geometry {
public:

  static float rotate(float x0, float y0, float phi, float & r, float &d, int&oct) {
    using namespace GeometryConstants;
   // see below for the equations....
    // reduce phi (see sincos)
    int o;
    phi = cephes::reduce2octant(phi,o);
    float vert = (o+1)&2;
    if(vert) {
      float tmp = y0;
      y0 = -x0; x0=tmp;
    }
    if (float((o+1)&7)>3.5f) {
      y0=-y0; x0=-x0;
    } 
    d = y0/cosPIo8;
    float rr = d*sinPIo8;
    if (phi<0. ) rr = -rr;
    r = rr + x0;    
    oct =o;
    return phi;
  }



  static void cross0(float phi, float tgl, float r, float &d, float &z) {
    using namespace GeometryConstants;
    // we are in first (or last) octant
    float s,c; cephes::sincosf0(fabs(phi),s,c);
    float rod = r/(sinPIo8*s+cosPIo8*c);
    d =  s*rod; if (phi<0.f) d = -d;
    z = tgl*rod*cosPIo8;
 
  }


  // assume x0 and y0 to be in the same octant than phi
  static void cross(float x0, float y0, float z0, float phi, float tgl, float r, float &d, float &z, int&oct) {
    using namespace GeometryConstants;
    // move in first octant
    
    

    x0 = fabs(x0);
    y0 = fabs(y0);
    float tmp =0.0f;
    if (y0>x0) {
      tmp = y0;
      y0=x0; x0=tmp;
    }
    
    
    // reduce phi (see sincos)
    int o;
    phi = cephes::reduce2octant(phi,o);
    oct = o;
    

    float s,c; cephes::sincosf0(fabs(phi),s,c);
    // meaningless code to inhibit vectorization
    // if (oct==0) phi = phi+0.01f; 
    // else if (phi>1.f) { oct-=1; phi =  phi-0.01f;}

    // track is x= x0+L*c; y = y0+L*s; z= z0+L*tgl
    // octagon side is x= r -d*sinPIo8; y = d*cosPIo8;
    // 

   
    float od = 1.f/(sinPIo8*s+cosPIo8*c);
    float dd = (s*(r-x0) +c*y0)*od;
    z = z0 + tgl*od*(cosPIo8*(r-x0)+y0*sinPIo8);
    //  if (phi<0) d = r/(cosPIo8+sinPIo8)-d;
    if (phi<0) dd = (cosPIo4/cosPIo8)*r-dd;
    //if (phi<0) d = 7.653662e-01 -d;
    d=dd;
  }

};

#endif
