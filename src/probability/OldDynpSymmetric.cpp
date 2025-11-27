#include "probability/OldDynpSymmetric.h"

#include <algorithm>

double OldDynpSymmetric::get_multiplier_m(int v_m) const
{
  if (degree_m_[v_m] >= (expected_m_ + 0.5))
  {
    return (degree_m_[v_m] >= (expected_m_ + 1.0)) ? 0.9 : 0.5;
  }
  return 6.7 - 5.6 * (static_cast<double>(degree_m_[v_m]) / (expected_m_ + 0.5));
}

double OldDynpSymmetric::get_multiplier_n(int v_n) const
{
  if (degree_n_[v_n] >= (expected_n_ + 0.5))
  {
    return (degree_m_[v_n] >= (expected_n_ + 1.0)) ? 0.9 : 0.5;
  }
  return 6.7 - 5.6 * (static_cast<double>(degree_n_[v_n]) / (expected_n_ + 0.5));
}

double OldDynpSymmetric::compute_m_term(int v_m, int v_n) const
{
  double mult_m = get_multiplier_m(v_m);

  double term = expected_percent_ * 1.1 -
                (static_cast<double>(degree_n_[v_n]) / (expected_n_ + 0.5)) * expected_percent_;

  return std::min(mult_m * term, 0.8);
}

double OldDynpSymmetric::compute_n_term(int v_m, int v_n) const
{
  double mult_n = get_multiplier_n(v_n);

  double term = expected_percent_ * 1.1 -
                (static_cast<double>(degree_m_[v_m]) / (expected_m_ + 0.5)) * expected_percent_;

  return std::min(mult_n * term, 0.8);
}

double OldDynpSymmetric::apply_mean(double a, double b) const
{
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

  return std::sqrt(a * b);
}

double OldDynpSymmetric::get_p(int v_m, int v_n)
{
  double m_term = compute_m_term(v_m, v_n);
  double n_term = compute_n_term(v_m, v_n);

  return apply_mean(m_term, n_term);
}