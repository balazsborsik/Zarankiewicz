#ifndef K33STORE_H
#define K33STORE_H

#include <vector>

#include "structure/Kst.h"
#include "structure/KstStore.h"

class K33Store : public KstStore
{
 private:
  Graph edges_in_circles_;
  std::vector<Kst<3, 3>> circles_;

 public:
  K33Store() : edges_in_circles_(Constants::MAX_SIZE, Constants::MAX_SIZE) {}
  ~K33Store() override = default;
  int getS() const { return 3; }
  int getT() const { return 3; }
  bool empty() { return circles_.empty(); }
  void clear() override;
  bool createsKst(const Graph &adj, int u, int v) const override;
  void storeKst(const Graph &adj, int u, int v) override;
  void reevalCircles(const Graph &adj) override;
  bool reflipCircle(Graph &adj, Probabilities &prob) override;
};

#endif  // K33STORE_H
