#pragma once

#include "structure/Kst.h"
#include "structure/KstStore.h"

class K22Store : public KstStore
{
 private:
  static constexpr int S_ = 2;
  static constexpr int T_ = 2;

  Graph edges_in_circles_;
  std::vector<Kst<2, 2>> circles_;

 public:
  K22Store() : edges_in_circles_(Constants::MAX_SIZE, Constants::MAX_SIZE) {}
  ~K22Store() override = default;
  int getS() const { return S_; }
  int getT() const { return T_; }
  bool empty() { return circles_.empty(); }
  void clear() override;
  bool createsKst(const Graph& adj, int u, int v) const override;
  void storeKst(const Graph& adj, int u, int v) override;
  void reevalCircles(const Graph& adj) override;
  bool reflipCircle(Graph& adj, Probabilities& prob) override;
};