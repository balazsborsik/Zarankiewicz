#ifndef EXISTINGGRAPHS_H
#define EXISTINGGRAPHS_H

#include <fstream>

#include "config/Constants.h"
#include "io/FileManager.h"
#include "structure/KstStore.h"

class ExistingGraphs
{
 private:
  std::array<Graph, Constants::MAX_GRAPHS_TO_SAVE> startingGraphs_;
  int realSize = 0;

  void addVertexToGraphM(Graph& inputGraph, const KstStore* kstStore);

  void returnExistingGraphs(int m, int n);
  void addVertexToM(int m, int n, const KstStore* kstStore);
  void addVertexToN(int m, int n, const KstStore* kstStore);

 public:
  ExistingGraphs(int m, int n, int operationType = 0, const KstStore* kstStore = nullptr);
  Graph getStartingGraph(int iteration) const;
};

#endif  // EXISTINGGRAPHS_H
