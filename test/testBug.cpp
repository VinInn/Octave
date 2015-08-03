const int N=1024;
float __attribute__ ((aligned(16))) a[N];
float __attribute__ ((aligned(16))) b[N];
float __attribute__ ((aligned(16))) c[N];
float __attribute__ ((aligned(16))) d[N];

void loop1() {
  for (int i=0; i!=N; ++i) {
    d[i]=a[i]+b[i];
    if (c[i]<0) d[i] = -d[i];
  }
}

void loop2() {
  for (int i=0; i!=N; ++i) {
    float tmp = a[i]+b[i];
    if (c[i]<0) tmp = -tmp;
    d[i]=tmp;
  }
}

void loop3() {
  for (int i=0; i!=N; ++i) {
    d[i] = (c[i]>0) ? a[i]+b[i] : -a[i]-b[i];
  }
}

void loop4() {
  for (int i=0; i!=N; ++i) {
    float tmp = a[i]+b[i];
     d[i] = (c[i]>0) ? tmp : -tmp;
  }
}


int main() {

  return 0;
}
