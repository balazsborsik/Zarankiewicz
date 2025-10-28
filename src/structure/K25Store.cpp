#include "structure/K25Store.h"

#include <algorithm>

void K25Store::clear()
{
  circles_.clear();
  edges_in_circles_.reset();
}

bool K25Store::createsKst(const Graph& adj, int u, int v) const
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
          for (int v3 = v2 + 1; v3 < n; ++v3)
          {
            if (adj[u][v3] && adj[u2][v3])
            {
              for (int v4 = v3 + 1; v4 < n; ++v4)
              {
                if (adj[u][v4] && adj[u2][v4])

                  for (int v5 = v4 + 1; v5 < n; ++v5)
                  {
                    if (adj[u][v5] && adj[u2][v5])
                    {
                      return true;
                    }
                  }
              }
            }
          }
        }
      }
    }
  }
  return false;
}

void K25Store::storeKst(const Graph& adj, int u, int v)
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
          for (int v3 = v2 + 1; v3 < n; ++v3)
          {
            if (adj[u][v3] && adj[u2][v3])
            {
              for (int v4 = v3 + 1; v4 < n; ++v4)
              {
                if (adj[u][v4] && adj[u2][v4])
                {
                  for (int v5 = v4 + 1; v5 < n; ++v5)
                  {
                    if (adj[u][v5] && adj[u2][v5])
                    {
                      Kst<2, 5> created({u, u2}, {v, v2, v3, v4, v5});
                      for (int i = 0; i < 2; ++i)
                      {
                        for (int j = 0; j < 5; ++j)
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
          }
        }
      }
    }
  }
}

void K25Store::reevalCircles(const Graph& adj)
{
  circles_.erase(
      std::remove_if(circles_.begin(), circles_.end(),
                     [&adj, this](const Kst<2, 5>& circle)
                     {
                       for (int i = 0; i < 2; ++i)
                       {
                         for (int j = 0; j < 5; ++j)
                         {
                           if (!adj[circle.u_arr[i]][circle.v_arr[j]])
                           {
                             for (int i = 0; i < 2; ++i)
                               for (int j = 0; j < 5; ++j)
                                 edges_in_circles_.adj[circle.u_arr[i]][circle.v_arr[j]]--;
                             return true;
                           }
                         }
                       }
                       return false;
                     }),
      circles_.end());
}

bool K25Store::reflipCircle(Graph& adj, Probabilities& prob)
{
  if (circles_.empty()) return false;
  std::pair<int, int> edge = std::make_pair(circles_[0].u_arr[0], circles_[0].v_arr[0]);
  int degree = 0;
  for (const auto& elm : circles_)
  {
    for (int i = 0; i < 2; ++i)
    {
      for (int j = 0; j < 5; ++j)
      {
        int u = elm.u_arr[i];
        int v = elm.v_arr[j];
        if (edges_in_circles_[u][v] > degree ||
            (edges_in_circles_[u][v] == degree &&
             (u < edge.first || (!(edge.first < u) && v < edge.second))))
        {
          degree = edges_in_circles_[u][v];
          edge.first = u;
          edge.second = v;
        }
      }
    }
  }
  adj.removeEdge(edge.first, edge.second);
  prob.delete_edge(edge.first, edge.second);
  return true;
}