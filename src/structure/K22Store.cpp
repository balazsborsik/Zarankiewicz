#include "structure/K22Store.h"

#include <algorithm>

#include "structure/KstHelper.h"

void K22Store::clear()
{
  circles_.clear();
  edges_in_circles_.reset();
}

bool K22Store::createsKst(const Graph& adj, int u, int v) const
{
  int m = adj.m;
  int n = adj.n;
  for (int u2 = 0; u2 < m; ++u2)
  {
    if (adj[u2][v])
    {
      for (int v2 = 0; v2 < n; ++v2)
      {
        if (adj[u][v2] && adj[u2][v2])
        {
          return true;
        }
      }
    }
  }
  return false;
}

void K22Store::storeKst(const Graph& adj, int u, int v)
{
  int m = adj.m;
  int n = adj.n;
  for (int u2 = 0; u2 < m; ++u2)
  {
    if (adj[u2][v])
    {
      for (int v2 = 0; v2 < n; ++v2)
      {
        if (adj[u][v2] && adj[u2][v2])
        {
          Kst<S_, T_> created({u, u2}, {v, v2});
          for (int i = 0; i < S_; ++i)
          {
            for (int j = 0; j < T_; ++j)
            {
              edges_in_circles_.adj[created.u_arr[i]][created.v_arr[j]]++;
            }
          }
          circles_.push_back(created);
        }
      }
    }
  }
}

void K22Store::reevalCircles(const Graph& adj)
{
  KstHelper::reevalCircles<S_, T_>(circles_, edges_in_circles_, adj);
}

bool K22Store::reflipCircle(Graph& adj, Probabilities& prob)
{
  return KstHelper::reflipCircle<S_, T_>(circles_, edges_in_circles_, adj, prob);
}