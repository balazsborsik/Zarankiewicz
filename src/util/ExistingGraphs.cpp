#include "util/ExistingGraphs.h"

#include <cmath>
#include <stdexcept>

void ExistingGraphs::returnExistingGraphs(int m, int n)
{
  startingGraphs_ = FileManager::getExistingGraphs(m, n);
  if (startingGraphs_[0].edges == 0)
  {
    startingGraphs_[0].m = m;
    startingGraphs_[0].n = n;
    realSize = 1;  // because if we have no graphs we always give back the first in the array
    return;
  }
  for (const Graph& elm : startingGraphs_)
  {
    if (elm.edges > 0)
    {
      ++realSize;
    }
  }
};

void ExistingGraphs::addVertexToGraphM(Graph& graph)
{
  int n = graph.n;
  int m = ++graph.m;
  int order[Constants::MAX_SIZE] = {};
  for (int i = 0; i < n; ++i)
  {
    order[i] = i;
  }
  int chosenVertices[Constants::MAX_SIZE] = {};
  int bestSize = 0;
  int iterations = pow(m, 0.5) * n / 8 + 5;
  for (int k = 0; k < iterations; ++k)
  {
    std::random_shuffle(std::begin(order), std::begin(order) + n);
    int currentChosen[Constants::MAX_SIZE] = {};
    int neighbours[Constants::MAX_SIZE] = {};
    int currentSize = 0;
    for (int i = 0; i < n; ++i)
    {
      int vertex = order[i];
      if (!createsKst(graph, m - 1, vertex))
      {
        currentChosen[currentSize++] = vertex;
        graph.addEdge(m - 1, vertex);
      }
    }
    if (currentSize > bestSize)
    {
      bestSize = currentSize;
      for (int i = 0; i < bestSize; ++i)
      {
        chosenVertices[i] = currentChosen[i];
      }
    }
    for (int i = 0; i < currentSize; i++)
    {
      graph.removeEdge(m - 1, currentChosen[i]);
    }
  }
  for (int i = 0; i < bestSize; ++i)
  {
    graph.addEdge(m - 1, chosenVertices[i]);
  }
};

void ExistingGraphs::addVertexToM(int m, int n)
{
  startingGraphs_ = FileManager::getExistingGraphs(
      m - 1, n);  // its safe because FileManager returns an array even if we call with 0
  if (startingGraphs_[0].edges == 0)
  {
    returnExistingGraphs(m, n);
    return;
  }
  for (Graph& elm : startingGraphs_)
  {
    if (elm.edges > 0)
    {
      ++realSize;
      addVertexToGraphM(elm);
    }
  }
};

ExistingGraphs::ExistingGraphs(int m, int n, int operationType)
{
  switch (operationType)
  {
    case 0:
      returnExistingGraphs(m, n);
      break;
    case 1:
      addVertexToM(m, n);
      break;
    case 2:
      addVertexToN(m, n);
      break;
    default:
      throw std::invalid_argument("Invalid operation type in ExistingGraphs constructor");
      break;
  }
}

Graph ExistingGraphs::getStartingGraph(int iteration) const
{
  return startingGraphs_[iteration % realSize];
}
