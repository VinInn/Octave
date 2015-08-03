#include<cmath>
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


void fake2(int N) {
  for (int i=0; i!=N; i+=2) {
   float r1 = ai[i], r2=ai[i+1];
   float u = std::sqrt(r1);
   float v = 1.f/r2;
   ao[i] = u*v; ao[i+1] = u*r2;
  }
}


int main() { return 0;}
