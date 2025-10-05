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
  ExistingGraphs(int m, int n, int operationType = 0)
  {
    startingGraphs_ = FileManager::getExistingGraphs(m, n);
    for (const auto& elm : startingGraphs_)
    {
      if (elm.first > 0)
      {
        ++realSize;
      }
    }
    if (realSize == 0)
      realSize++;  // because if we have no graphs we always give back the first in the array
  }

  Graph getStartingGraph(int iteration) { return startingGraphs_[iteration % realSize].second; }
};

#endif  // EXISTINGGRAPHS_H
