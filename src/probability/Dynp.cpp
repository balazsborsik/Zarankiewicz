#include "probability/Dynp.h"

#include "config/Constants.h"

double Dynp::fillFactor(int degree, double expected) const
{
  return expected_percent_ * (expected / (double)(degree + 0.01));  // to avoid division by zero
}

double Dynp::get_p(int v_m, int v_n)
{
  double fill_m = fillFactor(degree_m_[v_m], expected_m_);
  double fill_n = fillFactor(degree_n_[v_n], expected_n_);
  return std::min(fill_m * fill_n, 0.9);  // TODO: switch it to config if it can read double values
}