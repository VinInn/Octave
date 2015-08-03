#ifndef carray_H
#define carray_H
/*  an array that can be filled and used as constexpr
original implementation by Daniel Krügler
 */
#include <array>
#include <utility>
#include <type_traits>

// Some meta template stuff
namespace carray_details {
  template<int...> struct indices{};
  
  template<int I, class IndexTuple, int N>
  struct make_indices_impl;
  
  template<int I, int... Indices, int N>
  struct make_indices_impl<I, indices<Indices...>, N>
  {
    typedef typename make_indices_impl<I + 1, indices<Indices..., I>,
				       N>::type type;
  };
  
  template<int N, int... Indices>
  struct make_indices_impl<N, indices<Indices...>, N> {
    typedef indices<Indices...> type;
  };

  template<int N>
  struct make_indices : make_indices_impl<0, indices<>, N> {};
} 
// end of stuff


template<class T, std::size_t N>
struct carray {
  T data[N];
  constexpr const T& operator[](std::size_t i) const { return data[i]; }
  T& operator[](std::size_t i) { return data[i]; }
  constexpr std::size_t size() { return N; }
  T* begin() { return data; }
  T* end() { return data + N; }
  const T* begin() const { return data; }
  const T* end() const { return data + N; }
};



template<int I0, class F, int... I>
constexpr std::array<decltype(std::declval<F>()(std::declval<int>())), sizeof...(I)>
do_make_carray(F f, carray_details::indices<I...>)
{
  return std::array<decltype(std::declval<F>()(std::declval<int>())),
sizeof...(I)>{{ f(I0 + I)... }};
}

template<int N, int I0 = 0, class F>
constexpr std::array<decltype(std::declval<F>()(std::declval<int>())), N>
make_carray(F f) {
  return do_make_carray<I0>(f, typename carray_details::make_indices<N>::type());
}





#endif
