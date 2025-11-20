#ifndef OLD_DYNP_SYMMETRIC_H
#define OLD_DYNP_SYMMETRIC_H

#include "probability/Probabilities.h"

enum class OldDynpSymmetricType
{
  HARMONIC_MEAN,
  GEOMETRIC_MEAN,
  ARITHMETIC_MEAN,
  QUADRATIC_MEAN
};

class OldDynpSymmetric : public Probabilities
{
 private:
  OldDynpSymmetricType meanType = OldDynpSymmetricType::GEOMETRIC_MEAN;

 public:
  OldDynpSymmetric(int m, int n, int s, int t,
                   OldDynpSymmetricType type = OldDynpSymmetricType::GEOMETRIC_MEAN)
      : Probabilities(m, n, s, t)
  {
    meanType = type;
  }
  OldDynpSymmetric(const Graph& graph, int s, int t,
                   OldDynpSymmetricType type = OldDynpSymmetricType::GEOMETRIC_MEAN)
      : Probabilities(graph, s, t)
  {
    meanType = type;
  }
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

    switch (meanType)
    {
      case OldDynpSymmetricType::HARMONIC_MEAN:
        return 2.0 / (1.0 / a + 1.0 / b);

      case OldDynpSymmetricType::GEOMETRIC_MEAN:
        return std::sqrt(a * b);

      case OldDynpSymmetricType::ARITHMETIC_MEAN:
        return (a + b) / 2.0;

      case OldDynpSymmetricType::QUADRATIC_MEAN:
        return std::sqrt((a * a + b * b) / 2.0);
    }
    return std::sqrt(a * b);  // default GEOMETRIC_MEAN
  }
};

#endif  // OLD_DYNP_SYMMETRIC_H
