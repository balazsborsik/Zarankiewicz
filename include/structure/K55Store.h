#ifndef K55STORE_H
#define K55STORE_H

#include <vector>

#include "structure/Kst.h"
#include "structure/KstStore.h"

class K55Store : public KstStore
{
 private:
  Graph edges_in_circles_;
  std::vector<Kst<5, 5>> circles_;

 public:
  K55Store() : edges_in_circles_(Constants::MAX_SIZE, Constants::MAX_SIZE) {}
  ~K55Store() override = default;
  int getS() const { return 5; }
  int getT() const { return 5; }
  bool empty() { return circles_.empty(); }
  void clear() override;
  bool createsKst(const Graph &adj, int u, int v) const override;
  void storeKst(const Graph &adj, int u, int v) override;
  void reevalCircles(const Graph &adj) override;
  bool reflipCircle(Graph &adj, Probabilities &prob) override;
};

#endif  // K55STORE_H
