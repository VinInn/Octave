#ifndef STAT_H
#define STAT_H
#include<limits>
#include<cmath>

/* the usual mini statistics
 */
template<typename T>
struct Stat {
  constexpr Stat() : n(0),tot(0),tot2(0),amin(std::numeric_limits<T>::max()),amax(std::numeric_limits<T>::min()){}
  void add(T t) {
    n++;
    tot+=t;
    tot2+=t*t;
    if (t<amin) amin=t;
    if (t>amax) amax=t;
  }
  constexpr T ave() const { return n ? tot/T(n) : T(0);}
  constexpr T rms() const { return (n>1) ? std::sqrt((tot2 - tot*tot/T(n))/T(n-1)) : T(0);}

  int n;
  T tot;
  T tot2;
  T amin;
  T amax;

};
#endif // STAT_H
