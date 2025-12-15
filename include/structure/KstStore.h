#pragma once

#include <vector>

#include "core/Graph.h"
#include "probability/Probabilities.h"
#include "structure/Kst.h"

class KstStore
{
 public:
  virtual ~KstStore() = default;
  virtual int getS() const = 0;
  virtual int getT() const = 0;
  virtual void clear() = 0;
  virtual bool empty() = 0;
  virtual bool createsKst(const Graph& adj, int u, int v) const = 0;
  virtual void storeKst(const Graph& adj, int u, int v) = 0;
  virtual void reevalCircles(const Graph& adj) = 0;
  virtual bool reflipCircle(Graph& adj, Probabilities& prob) = 0;
};
