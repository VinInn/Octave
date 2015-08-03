
static const float SQRTHF = 0.707106781186547524f;
#include<cmath>
#include<iostream>
#include<cstdio>


inline float i2f(int x) {
    union { float f; int i; } tmp;
    tmp.i=x;
    return tmp.f;
  }
  
  
  inline int f2i(float x) {
    union { float f; int i; } tmp;
    tmp.f=x;
    return tmp.i;
  }


float myf(float x, int& e) {
  // x = frexpf( x, &e );
    
  // exponent (-1)
  int n = f2i(x); 
  e = (n >> 23)-126;
    
  // fractional part
  constexpr int inv_mant_mask =  ~0x7f800000;
  const int p05 = f2i(0.5f);
  n &= inv_mant_mask;
  n |= p05;
  return i2f(n);
}

void comp(float x) {
  float y0, y;
  int e0, e;
  y0 = frexpf( x, &e0 );
  y = myf( x, e );

  printf("%e  %d %e   %d %e\n",x,e0,y0,e,y);
  
}


int main() {

  comp(1e-1);
  comp(SQRTHF);
  comp(1);
  comp(10);

  return 0;
}
