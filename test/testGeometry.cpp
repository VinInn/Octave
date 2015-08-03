#include "Geometry.h"
#include <iostream>
#include <cstdio>

#include "testAids.h"


int main() {

  const int N=64;
  float phi[2*N];
  int oct[2*N];
  float d[2*N];
  float r[2*N];
  float p[2*N];
  float z[2*N];
  float d0[2*N];
  float r0[2*N];
  float d1[2*N];
  float r1[2*N];
  float z1[2*N];
  float d2[2*N];
  float r2[2*N];
  float z2[2*N];

  float a[2*N];
  float b[2*N];
  float c[2*N];

  for (int i=0; i!=N; ++i) a[N+i]=a[i]= c[N+i]=c[i]=0.f;
  for (int i=0; i!=N; ++i) b[N+i]=b[i]=1.f;

  phi[N]=phi[0] = -2.*3.141592653589793238;
  for (int i=1; i!=N; ++i) phi[N+i]=phi[i]= phi[i-1]+ 3.141592653589793238f/16.f;
  testAids(a,b,phi); 

  for (int i=0; i!=N; ++i) {
    Geometry::cross(a[i],a[i],a[i],phi[i],b[i],b[i],d[i],z[i],oct[i]);
  }

  for (int i=0; i!=N; ++i) {
    float s,c; cephes::sincosf(phi[i],s,c); // a[i]=s;b[i]=c;
    Geometry::cross(0.5f*c,0.5f*s,0.5f,phi[i],b[i],b[i],d[N+i],z[N+i],oct[N+i]);
  }
  // for (int i=0; i!=64; ++i) { printf("s c %g %g",a[i],b[i]);}


  for (int i=0; i!=64; ++i) {
    int o; float p = cephes::reduce2octant(phi[i],o);
     float s,c; cephes::sincosf0(p,s,c);
     printf("s c %g %g %d %e %e\n",180.*phi[i]/3.141597,180.*p/3.141597, o, s-sin(p),c-cos(p));
  }
  for (int i=0; i!=64; ++i) {
    printf("x-z %g %d %g %g\n",phi[i],oct[i],d[i],z[i]); 
    printf("x-z %g %d %g %g\n",phi[N+i],oct[N+i],d[N+i],z[N+i]); 
  }


  for (int i=0; i!=64; ++i) {
    float s,c; cephes::sincosf(phi[i],s,c);
    p[i] = Geometry::rotate(0.5f*c,0.5f*s,phi[i],d[i],r[i],oct[i]);
    p[i+N] = Geometry::rotate(-0.5f*c,-0.5f*s,phi[N+i],d[i+N],r[i+N],oct[i+N]);
  }
  for (int i=0; i!=64; ++i) {
    printf("d-r %g %d %g %g\n",p[i],oct[i],d[i],r[i]); 
    printf("d-r %g %d %g %g\n",p[N+i],oct[N+i],d[N+i],r[N+i]); 
  }

  // verify
  float x0=0.1f, y0=0.1f;
  float tgl=1.f; float rf=2.f;
  for (int i=0; i!=64; ++i) {
    float s,c; cephes::sincosf(phi[i],s,c);
    Geometry::cross(x0*c,y0*s,0.f,phi[i],tgl,rf,d[i],z[i],oct[i]);
    p[i] = Geometry::rotate(x0*c,y0*s,phi[i],r0[i],d0[i],oct[i]);
    Geometry::cross0(p[i],tgl,rf-r0[i],d2[i],z2[i]); d2[i]+=d0[i];
  }
  for (int i=0; i!=64; ++i) {
    printf("d-d %g %d %g %g   %g %g\n",p[i],oct[i],r0[i],d0[i],d[i],d2[i]); 
  }


  return 0;
}
