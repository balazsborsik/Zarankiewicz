#ifndef KST_H
#define KST_H

#include <array>
#include <utility>

template <int S, int T>
struct Kst
{
  std::array<int, S> u_arr;
  std::array<int, T> v_arr;

  Kst(const std::array<int, S>& U, const std::array<int, T>& V) : u_arr(U), v_arr(V) {}
};

#endif  // KST_H
