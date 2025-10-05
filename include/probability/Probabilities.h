#ifndef PROBABILITIES_H
#define PROBABILITIES_H

#include "core/Graph.h"
#include "util/Util.h"

class Probabilities
{
 protected:
  int m_, n_;
  int s_, t_;
  int degree_m_[Constants::MAX_SIZE] = {};
  int degree_n_[Constants::MAX_SIZE] = {};
  double expected_m_;
  double expected_n_;
  double expected_percent_;

 public:
  virtual ~Probabilities() = default;
  Probabilities(int m, int n, int s, int t) { init(m, n, s, t); }
  Probabilities(const Graph &graph, int s, int t) { init(graph.m, graph.n, s, t); };
  virtual void reInitialize(int m, int n, int s, int t) { init(m, n, s, t); }
  virtual void reInitialize(const Graph &graph, int s, int t) { init(graph.m, graph.n, s, t); }
  virtual void clear()
  {
    std::fill(degree_m_, degree_m_ + Constants::MAX_SIZE, 0);
    std::fill(degree_n_, degree_n_ + Constants::MAX_SIZE, 0);
  };
  virtual void delete_edge(int v_m, int v_n)
  {
    --degree_m_[v_m];
    --degree_n_[v_n];
  };
  virtual void add_edge(int v_m, int v_n)
  {
    ++degree_m_[v_m];
    ++degree_n_[v_n];
  };
  virtual double get_p(int v_m, int v_n) = 0;

 private:
  void init(int m, int n, int s, int t)
  {
    m_ = m;
    n_ = n;
    s_ = s;
    t_ = t;
    int upper_bound = Util::upperBound(m_, n_, s_, t_);
    expected_m_ = ((double)upper_bound / m_);
    expected_n_ = ((double)upper_bound / n_);
    expected_percent_ = (double)upper_bound / (n_ * m_);
    std::fill(degree_m_, degree_m_ + Constants::MAX_SIZE, 0);
    std::fill(degree_n_, degree_n_ + Constants::MAX_SIZE, 0);
  }
};

#endif  // PROBABILITIES_H
