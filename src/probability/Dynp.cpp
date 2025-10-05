#include "probability/Dynp.h"

#include "config/Constants.h"
#include "util/Util.h"

Dynp::Dynp(int m, int n, int s, int t) : Probabilities(m, n, s, t)
{
  init();
}

Dynp::Dynp(const Graph &graph, int s, int t) : Probabilities(graph, s, t)
{
  init();
}

void Dynp::init()
{
  int upper_bound = Util::upperBound(m_, n_, s_, t_);
  expected_m_ = ((double)upper_bound / m_);
  expected_n_ = ((double)upper_bound / n_);
  expected_percent_ = (double)upper_bound / (n_ * m_);
  std::fill(degree_m_, degree_m_ + Constants::MAX_SIZE, 0);
  std::fill(degree_n_, degree_n_ + Constants::MAX_SIZE, 0);
}

double Dynp::fillFactor(int degree, double expected) const
{
  return expected_percent_ * (expected / (double)(degree + 0.01));  // to avoid division by zero
}

double Dynp::get_p(int v_m, int v_n)
{
  double fill_m = fillFactor(degree_m_[v_m], expected_m_);
  double fill_n = fillFactor(degree_n_[v_n], expected_n_);
  return std::min(fill_m * fill_n * Constants::PROBABILITY_MULTIPLIER,
                  0.9);  // TODO: switch it to config if it can read double values
}

void Dynp::delete_edge(int v_m, int v_n)
{
  --degree_m_[v_m];
  --degree_n_[v_n];
}

void Dynp::add_edge(int v_m, int v_n)
{
  ++degree_m_[v_m];
  ++degree_n_[v_n];
}

void Dynp::clear()
{
  std::fill(degree_m_, degree_m_ + Constants::MAX_SIZE, 0);
  std::fill(degree_n_, degree_n_ + Constants::MAX_SIZE, 0);
}

void Dynp::reInitialize(int m, int n, int s, int t)
{
  Probabilities::reInitialize(m, n, s, t);
  init();
}

void Dynp::reInitailize(const Graph &graph, int s, int t)
{
  Probabilities::reInitailize(graph, s, t);
  init();
}
