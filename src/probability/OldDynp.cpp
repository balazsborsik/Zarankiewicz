#include "probability/OldDynp.h"

#include <algorithm>

double OldDynp::get_p(int v_m, int v_n)
{
  int n = degree_n_[v_n];
  return std::min(get_multiplier(v_m) *
                      (expected_percent_ * 1.1 - (n / (expected_n_ + 0.5)) * expected_percent_),
                  0.8);
}

double OldDynp::get_multiplier(int v_m) const
{
  if (degree_m_[v_m] >= (expected_m_ + 0.5))
  {
    if (degree_m_[v_m] == (expected_m_ + 0.5)) return 0.9;
    return 0.5;
  }
  return 6.7 - 5.6 * ((double)degree_m_[v_m] / (expected_m_ + 0.5));
}