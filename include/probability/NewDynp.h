#ifndef OLD_DYNP_H
#define OLD_DYNP_H

#include "probability/Probabilities.h"

class OldDynp : public Probabilities
{
 public:
  OldDynp(int m, int n, int s, int t) : Probabilities(m, n, s, t) {}
  OldDynp(const Graph& graph, int s, int t) : Probabilities(graph, s, t) {}
  double get_p(int v_m, int v_n) override;
};

#endif  // OLD_DYNP_H
