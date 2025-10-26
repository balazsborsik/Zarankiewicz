#ifndef OLD_DYNP_SYMMETRIC_H
#define OLD_DYNP_SYMMETRIC_H

#include "probability/Probabilities.h"

class OldDynpSymmetric : public Probabilities
{
 public:
  OldDynpSymmetric(int m, int n, int s, int t) : Probabilities(m, n, s, t) {}
  OldDynpSymmetric(const Graph& graph, int s, int t) : Probabilities(graph, s, t) {}
  double get_p(int v_m, int v_n) override
  {
    double mult_m;
    if (degree_m_[v_m] >= (expected_m_ + 0.5))
      mult_m = (degree_m_[v_m] == (expected_m_ + 0.5)) ? 0.9 : 0.5;
    else
      mult_m = 6.7 - 5.6 * (static_cast<double>(degree_m_[v_m]) / (expected_m_ + 0.5));

    double mult_n;
    if (degree_n_[v_n] >= (expected_n_ + 0.5))
      mult_n = (degree_n_[v_n] == (expected_n_ + 0.5)) ? 0.9 : 0.5;
    else
      mult_n = 6.7 - 5.6 * (static_cast<double>(degree_n_[v_n]) / (expected_n_ + 0.5));
    double a = std::min(
        mult_m * (expected_percent_ * 1.1 -
                  (static_cast<double>(degree_n_[v_n]) / (expected_n_ + 0.5)) * expected_percent_),
        0.8);

    double b = std::min(
        mult_n * (expected_percent_ * 1.1 -
                  (static_cast<double>(degree_m_[v_m]) / (expected_m_ + 0.5)) * expected_percent_),
        0.8);

    return sqrt(a) * sqrt(b);
  }
};

#endif  // OLD_DYNP_SYMMETRIC_H
