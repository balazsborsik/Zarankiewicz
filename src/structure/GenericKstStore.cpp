#include "structure/GenericKstStore.h"

#include <algorithm>

#include "config/Constants.h"
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
  std::vector<int> current_u;
  current_u.reserve(s_);
  current_u.push_back(u);

  std::vector<int> candidates;
  candidates.reserve(adj.m);
  for (int i = 0; i < adj.m; ++i)
  {
    if (i != u && adj[i][v])
    {
      candidates.push_back(i);
    }
  }

  if (candidates.size() < (size_t)(s_ - 1)) return false;

  return checkRecurseU(adj, candidates, 0, current_u, v);
}

void GenericKstStore::storeKst(const Graph& adj, int u, int v)
{
  std::vector<int> current_u;
  current_u.reserve(s_);
  current_u.push_back(u);

  std::vector<int> candidates;
  candidates.reserve(adj.m);
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
  circles_.erase(std::remove_if(circles_.begin(), circles_.end(),
                                [&adj, this](const DynamicKst& circle)
                                {
                                  for (int u_node : circle.u_arr)
                                  {
                                    for (int v_node : circle.v_arr)
                                    {
                                      if (!adj[u_node][v_node])
                                      {
                                        decrementCounts(circle);
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

bool GenericKstStore::checkRecurseU(const Graph& adj, const std::vector<int>& candidates,
                                    int start_idx, std::vector<int>& current_u, int fixed_v) const
{
  if (current_u.size() == (size_t)s_)
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

void GenericKstStore::storeRecurseU(const Graph& adj, const std::vector<int>& candidates,
                                    int start_idx, std::vector<int>& current_u, int fixed_v)
{
  if (current_u.size() == (size_t)s_)
  {
    std::vector<int> v_candidates;
    v_candidates.reserve(adj.n);

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

    if (v_candidates.size() < (size_t)(t_ - 1)) return;

    std::vector<int> current_v;
    current_v.reserve(t_);
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

void GenericKstStore::storeRecurseV(const std::vector<int>& candidates, int start_idx,
                                    const std::vector<int>& final_u, std::vector<int>& current_v)
{
  if (current_v.size() == (size_t)t_)
  {
    circles_.emplace_back(final_u, current_v);
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

void GenericKstStore::decrementCounts(const DynamicKst& circle)
{
  for (int u : circle.u_arr)
  {
    for (int v : circle.v_arr)
    {
      edges_in_circles_.adj[u][v]--;
    }
  }
}