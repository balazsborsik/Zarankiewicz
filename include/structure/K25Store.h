#ifndef K25STORE_H
#define K25STORE_H

#include <vector>

#include "structure/Kst.h"
#include "structure/KstStore.h"

class K25Store : public KstStore
{
 private:
  Graph edges_in_circles_;
  std::vector<Kst<2, 5>> circles_;

 public:
  K25Store() : edges_in_circles_(Constants::MAX_SIZE, Constants::MAX_SIZE) {}
  ~K25Store() override = default;
  int getS() const { return 2; }
  int getT() const { return 5; }
  bool empty() { return circles_.empty(); }
  void clear() override;
  bool createsKst(const Graph& adj, int u, int v) const override;
  void storeKst(const Graph& adj, int u, int v) override;
  void reevalCircles(const Graph& adj) override;
  bool reflipCircle(Graph& adj, Probabilities& prob) override;
};

#endif  // K25STORE_H
