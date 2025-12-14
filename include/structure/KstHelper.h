#pragma once

#include <algorithm>
#include <utility>
#include <vector>

#include "core/Graph.h"
#include "probability/Probabilities.h"
#include "structure/Kst.h"

namespace KstHelper
{

template <int S, int T>
void reevalCircles(std::vector<Kst<S, T>>& circles, Graph& edges_in_circles, const Graph& adj)
{
  circles.erase(
      std::remove_if(circles.begin(), circles.end(),
                     [&adj, &edges_in_circles](const Kst<S, T>& circle)
                     {
                       for (int i = 0; i < S; ++i)
                       {
                         for (int j = 0; j < T; ++j)
                         {
                           if (!adj[circle.u_arr[i]][circle.v_arr[j]])
                           {
                             for (int u_idx = 0; u_idx < S; ++u_idx)
                             {
                               for (int v_idx = 0; v_idx < T; ++v_idx)
                               {
                                 --edges_in_circles.adj[circle.u_arr[u_idx]][circle.v_arr[v_idx]];
                               }
                             }
                             return true;
                           }
                         }
                       }
                       return false;
                     }),
      circles.end());
}

template <int S, int T>
bool reflipCircle(std::vector<Kst<S, T>>& circles, const Graph& edges_in_circles, Graph& adj,
                  Probabilities& prob)
{
  if (circles.empty()) return false;

  int best_u = circles[0].u_arr[0];
  int best_v = circles[0].v_arr[0];
  int max_degree = edges_in_circles.adj[best_u][best_v];

  for (const auto& circle : circles)
  {
    for (int i = 0; i < S; ++i)
    {
      for (int j = 0; j < T; ++j)
      {
        int u = circle.u_arr[i];
        int v = circle.v_arr[j];

        int deg = edges_in_circles.adj[u][v];

        if (deg > max_degree)
        {
          max_degree = deg;
          best_u = u;
          best_v = v;
        }
        else if (deg == max_degree)
        {
          if (u < best_u || (u == best_u && v < best_v))
          {
            best_u = u;
            best_v = v;
          }
        }
      }
    }
  }

  adj.removeEdge(best_u, best_v);
  prob.delete_edge(best_u, best_v);
  return true;
}

}  // namespace KstHelper