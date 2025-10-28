#ifndef K56STORE_H
#define K56STORE_H

#include <vector>

#include "structure/Kst.h"
#include "structure/KstStore.h"

class K56Store : public KstStore
{
 private:
  Graph edges_in_circles_;
  std::vector<Kst<5, 6>> circles_;

 public:
  K56Store() : edges_in_circles_(Constants::MAX_SIZE, Constants::MAX_SIZE) {}
  ~K56Store() override = default;
  int getS() const { return 5; }
  int getT() const { return 6; }
  bool empty() { return circles_.empty(); }
  void clear() override;
  bool createsKst(const Graph& adj, int u, int v) const override;
  void storeKst(const Graph& adj, int u, int v) override;
  void reevalCircles(const Graph& adj) override;
  bool reflipCircle(Graph& adj, Probabilities& prob) override;
};

#endif  // K56STORE_H
