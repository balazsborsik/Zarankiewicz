#ifndef DYNP_H
#define DYNP_H

#include "Constants.h"
#include "Probabilities.h"

class Dynp : public Probabilities
{
 private:
  int degree_m_[Constants::MAX_SIZE] = {};
  int degree_n_[Constants::MAX_SIZE] = {};
  double expected_m_;
  double expected_n_;
  double expected_percent_;

  void init();
  double fillFactor(int degree, double expected) const;

 public:
  Dynp(int m, int n, int s, int t);
  Dynp(const Graph &graph, int s, int t);
  double get_p(int v_m, int v_n) override;
  void delete_edge(int v_m, int v_n) override;
  void add_edge(int v_m, int v_n) override;
  void clear() override;
  void reInitialize(int m, int n, int s, int t) override;
  void reInitailize(const Graph &graph, int s, int t) override;
};

#endif  // DYNP_H
