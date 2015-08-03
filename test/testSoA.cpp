struct SoA {
  int * __restrict__ a;
  float * __restrict__ b;
  float * __restrict__ c;
  int size;
};

struct SoB {
  int *  __restrict__ a;
  float * __restrict__ b;
  int size;
};


void foo0(SoA const & __restrict__ in, SoB & __restrict__ out) {
  int & k = out.size;
  for (int i=0; i!=in.size; ++i) {
    out.b[k] = in.c[i]+in.b[i];
    out.a[k] = in.a[i];
    ++k;
  }
}


void foo(SoA const & __restrict__ in, SoB & __restrict__ out, int & __restrict__ k) {
  int N=in.size;
  for (int i=0; i!=N; ++i) {
    out.b[k] = in.c[i]+in.b[i];
    out.a[k] = in.a[i];
    ++k;
  }
}

void foo2(SoA const & __restrict__ in, SoB & __restrict__ out, int & __restrict__ k) {
  int j=k;
  for (int i=0; i!=in.size; ++i) {
    out.b[j] = in.c[i]+in.b[i];
    out.a[j] = in.a[i];
    ++j;
  }
  k = j;
}


void foo3(SoA const & __restrict__ in, SoB & __restrict__ out, int & __restrict__ k) {
  int j=k;
  int N=in.size;
  for (int i=0; i!=N; ++i) {
    out.b[j] = in.c[i]+in.b[i];
    out.a[j] = in.a[i];
    ++j;
  }
  k = j;
}



void bar(SoA const & __restrict__ in, SoB & __restrict__ out, int N, int & __restrict__ k) {
  int j=k;
  for (int i=0; i!=N; ++i) {
    j++;
    out.b[j] = in.c[i]+in.b[i];
    out.a[j] = in.a[i];
  }
  k = j;
}



void loop(  int const *  __restrict__ in_a, 
	    float const * __restrict__ in_b,
	    float const * __restrict__ in_c,
	    int *  __restrict__ out_a,
	    float * __restrict__ out_b,
	    int N, int & k) {
  int j=k;
  for (int i=0; i!=N; ++i) {
    out_b[j] = in_c[i]+in_b[i];
    out_a[j] = in_a[i];
    j++;
  }
  k = j;
}

void loop2(SoA const & in, SoB & out, int & k) {
  loop(in.a,in.b,in.c,
       out.a,out.b, 
       in.size, k);
}

void loop3(SoA const & in, SoB & out, int & k) {
  int j=k;
  int N=in.size;
  for (int i=0; i!=N; ++i) {
    out.b[j] = in.c[i]+in.b[i];
    out.a[j] = in.a[i];
    ++j;
  }
  k = j;
}


void loop4(SoA const & inI, SoB & outI, int & k) {
  SoA const in = inI;
  SoB out = outI;
  int j=k;
  for (int i=0; i!=in.size; ++i) {
    out.b[j] = in.c[i]+in.b[i];
    out.a[j] = in.a[i];
    ++j;
  }
  k = j;
  outI = out;
}


int main() {
  return 0;
}
