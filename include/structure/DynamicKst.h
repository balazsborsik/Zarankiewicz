#ifndef DYNAMICKST_H
#define DYNAMICKST_H

#include <array>
#include <utility>
#include <vector>

struct DynamicKst
{
  std::vector<int> u_arr;
  std::vector<int> v_arr;

  DynamicKst(const std::vector<int>& u, const std::vector<int>& v) : u_arr(u), v_arr(v) {}
};

#endif  // DYNAMICKST_H
