struct Foo {
  float a;
  float b;

  virtual ~Foo(){}

  void compute1(float * __restrict__ x, float const * __restrict__ y, int N) const;
  void compute2(float * __restrict__ x, float const * __restrict__ y, int N) const;

};

void Foo::compute1(float * __restrict__ x, float const * __restrict__ y, int N) const {
  for (int i=0; i!=N; ++i)
    x[i] = a + b*y[i];
}

void Foo::compute2(float * __restrict__ x, float const * __restrict__ y, int N) const {
  float la=a, lb=b;
  for (int i=0; i!=N; ++i)
    x[i] = la + lb*y[i];
}


int main() {
  return 0;
}
