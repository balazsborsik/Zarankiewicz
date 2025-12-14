#ifndef KST_H
#define KST_H

#include <array>
#include <utility>

template <int S, int T>
struct Kst
{
  std::array<int, S> u_arr;
  std::array<int, T> v_arr;

  Kst() = default;

  Kst(const std::array<int, S>& U, const std::array<int, T>& V) : u_arr(U), v_arr(V) {}
  template <typename UContainer, typename VContainer>
  Kst(const UContainer& u_cont, const VContainer& v_cont)
  {
    for (int i = 0; i < S; ++i) u_arr[i] = u_cont[i];
    for (int i = 0; i < T; ++i) v_arr[i] = v_cont[i];
  }
};

#endif  // KST_H
