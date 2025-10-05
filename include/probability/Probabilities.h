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
  Probabilities(int m, int n, int s, int t);
  Probabilities(const Graph &graph, int s, int t);
  virtual void reInitialize(int m, int n, int s, int t);
  virtual void reInitialize(const Graph &graph, int s, int t);
  virtual void clear();
  virtual void delete_edge(int v_m, int v_n);
  virtual void add_edge(int v_m, int v_n);
  virtual double get_p(int v_m, int v_n) = 0;

 private:
  void init(int m, int n, int s, int t);
  void initDegreesFromGraph(const Graph &graph);
};

#endif  // PROBABILITIES_H
