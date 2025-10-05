#ifndef GRAPH_H
#define GRAPH_H

#include "config/Constants.h"

struct Graph
{
  int adj[Constants::MAX_SIZE][Constants::MAX_SIZE] = {};
  int edges = 0;
  int m, n;

  Graph() : m(Constants::MAX_SIZE), n(Constants::MAX_SIZE) {}
  Graph(int m, int n) : m(m), n(n) {}
  const int* operator[](int index) const;
  void reset();
  int hasEdge(int u, int v) const;
  void addEdge(int u, int v);
  void removeEdge(int u, int v);
};

#endif  // GRAPH_H
