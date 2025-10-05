#ifndef DYNP_H
#define DYNP_H

#include "probability/Probabilities.h"

class Dynp : public Probabilities
{
 public:
  Dynp(int m, int n, int s, int t) : Probabilities(m, n, s, t) {}
  Dynp(const Graph &graph, int s, int t) : Probabilities(graph, s, t) {}
  double get_p(int v_m, int v_n) override;

 private:
  double fillFactor(int degree, double expected) const;
};

#endif  // DYNP_H
