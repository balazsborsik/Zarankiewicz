#ifndef NEW_DYNP_H
#define NEW_DYNP_H

#include "probability/Probabilities.h"

class NewDynp : public Probabilities
{
 public:
  NewDynp(int m, int n, int s, int t) : Probabilities(m, n, s, t) {}
  NewDynp(const Graph& graph, int s, int t) : Probabilities(graph, s, t) {}
  double get_p(int v_m, int v_n) override;
};

#endif  // NEW_DYNP_H
