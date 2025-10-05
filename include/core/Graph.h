#ifndef GRAPH_H
#define GRAPH_H

#include <array>
#include <iostream>

#include "config/Constants.h"

struct Graph
{
  int adj[Constants::MAX_SIZE][Constants::MAX_SIZE] = {};
  int edges = 0;
  int m, n;

  Graph() : m(Constants::MAX_SIZE), n(Constants::MAX_SIZE) {}

  Graph(int m, int n) : m(m), n(n) {}

  const int* operator[](int index) const { return adj[index]; }

  void reset()
  {
    std::fill(&adj[0][0], &adj[0][0] + Constants::MAX_SIZE * Constants::MAX_SIZE, 0);
    edges = 0;
  }

  int hasEdge(int u, int v) const { return adj[u][v]; }
  void addEdge(int u, int v)
  {
    if (adj[u][v])
    {
      std::cout << "addEdge: edge already exists";
      return;
    }
    adj[u][v] = 1;
    ++edges;
  }

  void removeEdge(int u, int v)
  {
    if (!adj[u][v])
    {
      std::cout << "removeEdge: edge does not exist";
      return;
    }
    adj[u][v] = 0;
    --edges;
  }
};

#endif  // GRAPH_H
