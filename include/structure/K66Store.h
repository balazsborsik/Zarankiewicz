#ifndef K66STORE_H
#define K66STORE_H

#include <vector>

#include "structure/Kst.h"
#include "structure/KstStore.h"

class K66Store : public KstStore
{
 private:
  Graph edges_in_circles_;
  std::vector<Kst<6, 6>> circles_;

 public:
  K66Store() : edges_in_circles_(Constants::MAX_SIZE, Constants::MAX_SIZE) {}
  ~K66Store() override = default;
  int getS() const { return 6; }
  int getT() const { return 6; }
  bool empty() { return circles_.empty(); }
  void clear() override;
  bool createsKst(const Graph &adj, int u, int v) const override;
  void storeKst(const Graph &adj, int u, int v) override;
  void reevalCircles(const Graph &adj) override;
  bool reflipCircle(Graph &adj, Probabilities &prob) override;
};

#endif  // K66STORE_H
