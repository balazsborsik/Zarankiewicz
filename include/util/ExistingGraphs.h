#ifndef EXISTINGGRAPHS_H
#define EXISTINGGRAPHS_H

#include <fstream>

#include "config/Constants.h"
#include "io/FileManager.h"

class ExistingGraphs
{
 private:
  std::array<std::pair<int, Graph>, Constants::MAX_GRAPHS_TO_SAVE> startingGraphs_;
  int realSize = 0;

 public:
  ExistingGraphs(int m, int n, int operationType = 0);
  Graph getStartingGraph(int iteration) const;
};

#endif  // EXISTINGGRAPHS_H
