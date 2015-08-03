#include "carray.h"
#include <iostream>




constexpr float g(int i) { return -i + 0.5; }

int main()
{
  constexpr auto v = make_carray<100>(g); // OK
  constexpr auto e1 = v[1]; // OK
  for (auto i : v) {
    std::cout << i << std::endl;
  }
}
