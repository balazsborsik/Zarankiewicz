#include "structure/K35Store.h"

#include "config/Constants.h"
#include "structure/KstHelper.h"

K35Store::K35Store() : edges_in_circles_(Constants::MAX_SIZE, Constants::MAX_SIZE) {}

void K35Store::clear()
{
  circles_.clear();
  edges_in_circles_.reset();
}

bool K35Store::createsKst(const Graph& adj, int u, int v) const
{
  int u_cands[Constants::MAX_SIZE];
  int u_cands_size = 0;

  for (int i = 0; i < adj.m; ++i)
  {
    if (i != u && adj[i][v])
    {
      u_cands[u_cands_size++] = i;
    }
  }

  if (u_cands_size < (S_ - 1)) return false;

  for (int i = 0; i < u_cands_size; ++i)
  {
    for (int j = i + 1; j < u_cands_size; ++j)
    {
      int common_v_count = 0;
      for (int node_v = 0; node_v < adj.n; ++node_v)
      {
        if (node_v == v) continue;

        if (adj[u][node_v] && adj[u_cands[i]][node_v] && adj[u_cands[j]][node_v])
        {
          common_v_count++;
          if (common_v_count >= (T_ - 1)) return true;
        }
      }
    }
  }

  return false;
}

void K35Store::storeKst(const Graph& adj, int u, int v)
{
  int u_cands[Constants::MAX_SIZE];
  int u_cands_size = 0;

  for (int i = 0; i < adj.m; ++i)
  {
    if (i != u && adj[i][v])
    {
      u_cands[u_cands_size++] = i;
    }
  }

  if (u_cands_size < (S_ - 1)) return;

  int v_cands[Constants::MAX_SIZE];
  int v_cands_size;

  for (int i = 0; i < u_cands_size; ++i)
  {
    for (int j = i + 1; j < u_cands_size; ++j)
    {
      v_cands_size = 0;

      for (int node_v = 0; node_v < adj.n; ++node_v)
      {
        if (node_v == v) continue;

        if (adj[u][node_v] && adj[u_cands[i]][node_v] && adj[u_cands[j]][node_v])
        {
          v_cands[v_cands_size++] = node_v;
        }
      }

      if (v_cands_size < (T_ - 1)) continue;

      for (int a = 0; a < v_cands_size; ++a)
      {
        for (int b = a + 1; b < v_cands_size; ++b)
        {
          for (int c = b + 1; c < v_cands_size; ++c)
          {
            for (int d = c + 1; d < v_cands_size; ++d)
            {
              Kst<S_, T_> created({u, u_cands[i], u_cands[j]},
                                  {v, v_cands[a], v_cands[b], v_cands[c], v_cands[d]});

              for (int x = 0; x < S_; ++x)
              {
                for (int y = 0; y < T_; ++y)
                {
                  edges_in_circles_.adj[created.u_arr[x]][created.v_arr[y]]++;
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

void K35Store::reevalCircles(const Graph& adj)
{
  KstHelper::reevalCircles<S_, T_>(circles_, edges_in_circles_, adj);
}

bool K35Store::reflipCircle(Graph& adj, Probabilities& prob)
{
  return KstHelper::reflipCircle<S_, T_>(circles_, edges_in_circles_, adj, prob);
}
