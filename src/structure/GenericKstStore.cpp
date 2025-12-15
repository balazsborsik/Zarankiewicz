#include "structure/GenericKstStore.h"

#include <algorithm>

#include "probability/Probabilities.h"

GenericKstStore::GenericKstStore(int s, int t)
    : s_(s), t_(t), edges_in_circles_(Constants::MAX_SIZE, Constants::MAX_SIZE)
{
}

void GenericKstStore::clear()
{
  circles_.clear();
  edges_in_circles_.reset();
}

bool GenericKstStore::createsKst(const Graph& adj, int u, int v) const
{
  Fixvector<int, Constants::MAX_S> current_u;
  current_u.push_back(u);

  Fixvector<int, Constants::MAX_SIZE> candidates;
  for (int i = 0; i < adj.m; ++i)
  {
    if (i != u && adj[i][v])
    {
      candidates.push_back(i);
    }
  }

  if (candidates.size() < (s_ - 1)) return false;

  return checkRecurseU(adj, candidates, 0, current_u, v);
}

void GenericKstStore::storeKst(const Graph& adj, int u, int v)
{
  Fixvector<int, Constants::MAX_S> current_u;
  current_u.push_back(u);

  Fixvector<int, Constants::MAX_SIZE> candidates;
  for (int i = 0; i < adj.m; ++i)
  {
    if (i != u && adj[i][v])
    {
      candidates.push_back(i);
    }
  }

  storeRecurseU(adj, candidates, 0, current_u, v);
}

void GenericKstStore::reevalCircles(const Graph& adj)
{
  circles_.erase(
      std::remove_if(circles_.begin(), circles_.end(),
                     [&adj, this](const Kst<Constants::MAX_S, Constants::MAX_T>& circle)
                     {
                       for (int i = 0; i < s_; ++i)
                       {
                         for (int j = 0; j < s_; ++j)
                         {
                           if (!adj[circle.u_arr[i]][circle.v_arr[j]])
                           {
                             for (int u_idx = 0; u_idx < s_; ++u_idx)
                             {
                               for (int v_idx = 0; v_idx < t_; ++v_idx)
                               {
                                 --edges_in_circles_.adj[circle.u_arr[u_idx]][circle.v_arr[v_idx]];
                               }
                             }
                             return true;
                           }
                         }
                       }
                       return false;
                     }),
      circles_.end());
}

bool GenericKstStore::reflipCircle(Graph& adj, Probabilities& prob)
{
  if (circles_.empty()) return false;

  int best_u = circles_[0].u_arr[0];
  int best_v = circles_[0].v_arr[0];
  int max_degree = edges_in_circles_.adj[best_u][best_v];

  for (const auto& circle : circles_)
  {
    for (int u : circle.u_arr)
    {
      for (int v : circle.v_arr)
      {
        int deg = edges_in_circles_[u][v];

        if (deg > max_degree || (deg == max_degree && (u < best_u || (u == best_u && v < best_v))))
        {
          max_degree = deg;
          best_u = u;
          best_v = v;
        }
      }
    }
  }
  adj.removeEdge(best_u, best_v);
  prob.delete_edge(best_u, best_v);
  return true;
}

bool GenericKstStore::checkRecurseU(const Graph& adj,
                                    const Fixvector<int, Constants::MAX_SIZE>& candidates,
                                    int start_idx, Fixvector<int, Constants::MAX_S>& current_u,
                                    int fixed_v) const
{
  if (current_u.size() == s_)
  {
    int count = 1;
    for (int j = 0; j < adj.n; ++j)
    {
      if (j == fixed_v) continue;

      bool connected_to_all = true;
      for (int u : current_u)
      {
        if (!adj[u][j])
        {
          connected_to_all = false;
          break;
        }
      }
      if (connected_to_all)
      {
        count++;
        if (count >= t_) return true;
      }
    }
    return false;
  }

  for (int i = start_idx; i < candidates.size(); ++i)
  {
    current_u.push_back(candidates[i]);
    if (checkRecurseU(adj, candidates, i + 1, current_u, fixed_v)) return true;
    current_u.pop_back();
  }
  return false;
}

void GenericKstStore::storeRecurseU(const Graph& adj,
                                    const Fixvector<int, Constants::MAX_SIZE>& candidates,
                                    int start_idx, Fixvector<int, Constants::MAX_S>& current_u,
                                    int fixed_v)
{
  if (current_u.size() == s_)
  {
    Fixvector<int, Constants::MAX_SIZE> v_candidates;

    for (int j = 0; j < adj.n; ++j)
    {
      if (j == fixed_v) continue;
      bool connected_to_all = true;
      for (int u : current_u)
      {
        if (!adj[u][j])
        {
          connected_to_all = false;
          break;
        }
      }
      if (connected_to_all) v_candidates.push_back(j);
    }

    if (v_candidates.size() < (t_ - 1)) return;

    Fixvector<int, 6> current_v;
    current_v.push_back(fixed_v);
    storeRecurseV(v_candidates, 0, current_u, current_v);
    return;
  }

  for (int i = start_idx; i < candidates.size(); ++i)
  {
    current_u.push_back(candidates[i]);
    storeRecurseU(adj, candidates, i + 1, current_u, fixed_v);
    current_u.pop_back();
  }
}

void GenericKstStore::storeRecurseV(const Fixvector<int, Constants::MAX_SIZE>& candidates,
                                    int start_idx, const Fixvector<int, Constants::MAX_S>& final_u,
                                    Fixvector<int, Constants::MAX_T>& current_v)
{
  if (current_v.size() == t_)
  {
    circles_.emplace_back(final_u.arr, current_v.arr);
    for (int u : final_u)
    {
      for (int v : current_v)
      {
        edges_in_circles_.adj[u][v]++;
      }
    }
    return;
  }

  for (int i = start_idx; i < candidates.size(); ++i)
  {
    current_v.push_back(candidates[i]);
    storeRecurseV(candidates, i + 1, final_u, current_v);
    current_v.pop_back();
  }
}
