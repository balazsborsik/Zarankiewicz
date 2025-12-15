#pragma once

#include "structure/Kst.h"
#include "structure/KstStore.h"

class K33Store : public KstStore
{
 private:
  static constexpr int S_ = 3;
  static constexpr int T_ = 3;

  Graph edges_in_circles_;
  std::vector<Kst<S_, T_>> circles_;

 public:
  K33Store();
  ~K33Store() override = default;

  int getS() const override { return S_; }
  int getT() const override { return T_; }
  bool empty() override { return circles_.empty(); }
  void clear() override;

  bool createsKst(const Graph& adj, int u, int v) const override;
  void storeKst(const Graph& adj, int u, int v) override;

  void reevalCircles(const Graph& adj) override;
  bool reflipCircle(Graph& adj, Probabilities& prob) override;
};
