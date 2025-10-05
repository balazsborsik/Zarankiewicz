#include "util/ExistingGraphs.h"

ExistingGraphs::ExistingGraphs(int m, int n, int operationType)
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

Graph ExistingGraphs::getStartingGraph(int iteration) const
{
  return startingGraphs_[iteration % realSize].second;
}
