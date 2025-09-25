#ifndef GRAPH_H
#define GRAPH_H

#include <array>

#include "Constants.h"

struct Graph
{
  int adj[Constants::MAX_SIZE][Constants::MAX_SIZE] = {};
  int m, n;

  Graph() : m(Constants::MAX_SIZE), n(Constants::MAX_SIZE) {}

  Graph(int m, int n) : m(m), n(n) {}

  void reset() { std::fill(&adj[0][0], &adj[0][0] + Constants::MAX_SIZE * Constants::MAX_SIZE, 0); }

  int* operator[](int row) { return adj[row]; }

  const int* operator[](int row) const { return adj[row]; }
};

#endif  // GRAPH_H
