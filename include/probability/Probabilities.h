#ifndef PROBABILITIES_H
#define PROBABILITIES_H

#include "core/Graph.h"

class Probabilities
{
 protected:
  int m_, n_;
  int s_, t_;

 public:
  virtual ~Probabilities() = default;
  Probabilities(int m, int n, int s, int t) : m_(m), n_(n), s_(s), t_(t) {}
  Probabilities(const Graph &graph, int s, int t) : m_(graph.m), n_(graph.n), s_(s), t_(t) {};
  virtual void reInitialize(int m, int n, int s, int t)
  {
    m_ = m;
    n_ = n;
    s_ = s;
    t_ = t;
  }
  virtual void reInitailize(const Graph &graph, int s, int t)
  {
    m_ = graph.m;
    n_ = graph.n;
    s_ = s;
    t_ = t;
  }
  virtual void clear() = 0;
  virtual double get_p(int v_m, int v_n) = 0;
  virtual void delete_edge(int v_m, int v_n) = 0;
  virtual void add_edge(int v_m, int v_n) = 0;
};

#endif  // PROBABILITIES_H
