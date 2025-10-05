#include "core/Graph.h"

#include <array>
#include <iostream>

#include "config/Constants.h"

const int* Graph::operator[](int index) const
{
  return adj[index];
}

void Graph::reset()
{
  std::fill(&adj[0][0], &adj[0][0] + Constants::MAX_SIZE * Constants::MAX_SIZE, 0);
  edges = 0;
}

int Graph::hasEdge(int u, int v) const
{
  return adj[u][v];
}
void Graph::addEdge(int u, int v)
{
  if (adj[u][v])
  {
    std::cout << "addEdge: edge already exists";
    return;
  }
  adj[u][v] = 1;
  ++edges;
}

void Graph::removeEdge(int u, int v)
{
  if (!adj[u][v])
  {
    std::cout << "removeEdge: edge does not exist";
    return;
  }
  adj[u][v] = 0;
  --edges;
}