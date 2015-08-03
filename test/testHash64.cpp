#include "constHash64.h"
#include <cstring>

typedef unsigned long long Key;
inline constexpr Key hash(const char * name) { return ::hash64(name,strlen(name),0);}

int main(int j, char **)	{
  constexpr Key  bra = hash("bha");
  constexpr Key  ket = hash("ket");
 
  if (bra==j) return 1;
  return 0;
}
