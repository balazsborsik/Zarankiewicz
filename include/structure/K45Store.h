#ifndef K45STORE_H
#define K45STORE_H

#include <vector>

#include "structure/Kst.h"
#include "structure/KstStore.h"

class K45Store : public KstStore
{
 private:
  Graph edges_in_circles_;
  std::vector<Kst<4, 5>> circles_;

 public:
  K45Store() : edges_in_circles_(Constants::MAX_SIZE, Constants::MAX_SIZE) {}
  ~K45Store() override = default;
  int getS() const { return 4; }
  int getT() const { return 5; }
  bool empty() { return circles_.empty(); }
  void clear() override;
  bool createsKst(const Graph& adj, int u, int v) const override;
  void storeKst(const Graph& adj, int u, int v) override;
  void reevalCircles(const Graph& adj) override;
  bool reflipCircle(Graph& adj, Probabilities& prob) override;
};

#endif  // K45STORE_H
