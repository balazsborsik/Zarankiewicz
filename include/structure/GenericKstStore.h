#ifndef GENERIC_KST_STORE_H
#define GENERIC_KST_STORE_H

#include <vector>

#include "core/Graph.h"
#include "structure/DynamicKst.h"
#include "structure/KstStore.h"

class Probabilities;

class GenericKstStore : public KstStore
{
 private:
  int s_;
  int t_;
  std::vector<DynamicKst> circles_;
  Graph edges_in_circles_;

 public:
  GenericKstStore(int s, int t);
  ~GenericKstStore() override = default;

  int getS() const override { return s_; }
  int getT() const override { return t_; }
  bool empty() override { return circles_.empty(); }
  void clear() override;

  bool createsKst(const Graph& adj, int u, int v) const override;
  void storeKst(const Graph& adj, int u, int v) override;
  void reevalCircles(const Graph& adj) override;
  bool reflipCircle(Graph& adj, Probabilities& prob) override;

 private:
  bool checkRecurseU(const Graph& adj, const std::vector<int>& candidates, int start_idx,
                     std::vector<int>& current_u, int fixed_v) const;

  void storeRecurseU(const Graph& adj, const std::vector<int>& candidates, int start_idx,
                     std::vector<int>& current_u, int fixed_v);

  void storeRecurseV(const std::vector<int>& candidates, int start_idx,
                     const std::vector<int>& final_u, std::vector<int>& current_v);

  void decrementCounts(const DynamicKst& circle);
};

#endif  // GENERIC_KST_STORE_H