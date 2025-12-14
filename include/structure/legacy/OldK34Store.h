#pragma once

#include "structure/Kst.h"
#include "structure/KstStore.h"

class OldK34Store : public KstStore
{
 private:
  Graph edges_in_circles_;
  std::vector<Kst<3, 4>> circles_;

 public:
  OldK34Store() : edges_in_circles_(Constants::MAX_SIZE, Constants::MAX_SIZE) {}
  ~OldK34Store() override = default;
  int getS() const { return 3; }
  int getT() const { return 4; }
  bool empty() { return circles_.empty(); }
  void clear() override;
  bool createsKst(const Graph& adj, int u, int v) const override;
  void storeKst(const Graph& adj, int u, int v) override;
  void reevalCircles(const Graph& adj) override;
  bool reflipCircle(Graph& adj, Probabilities& prob) override;
};