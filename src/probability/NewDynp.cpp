#include "probability/NewDynp.h"

#include <algorithm>

double NewDynp::get_p(int v_m, int v_n)
{
  double expected_percent_adjusted =
      (expected_percent_ * n_ * m_ - edge_count_) / (n_ * m_ - edge_count_);
  double prob_A = (expected_m_ - degree_m_[v_m]) / (n_ - degree_m_[v_m]);
  double prob_B = (expected_n_ - degree_n_[v_n]) / (m_ - degree_n_[v_n]);
  return ((expected_percent_adjusted + prob_A) / 2.0 + (expected_percent_adjusted + prob_B) / 2.0) /
         2.0;
}