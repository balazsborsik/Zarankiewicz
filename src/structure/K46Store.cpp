#include "structure/K46Store.h"

#include "config/Constants.h"
#include "structure/KstHelper.h"

K46Store::K46Store() : edges_in_circles_(Constants::MAX_SIZE, Constants::MAX_SIZE) {}

void K46Store::clear()
{
  circles_.clear();
  edges_in_circles_.reset();
}

bool K46Store::createsKst(const Graph& adj, int u, int v) const
{
  int m = adj.m;
  int n = adj.n;
  for (int u2 = 0; u2 < m; ++u2)
  {
    if (u2 == u) continue;
    if (adj[u2][v])
    {
      for (int v2 = 0; v2 < n; ++v2)
      {
        if (v2 == v) continue;
        if (adj[u][v2] && adj[u2][v2])
        {
          for (int u3 = u2 + 1; u3 < m; ++u3)
          {
            if (u3 == u) continue;
            if (adj[u3][v] && adj[u3][v2])
            {
              for (int v3 = v2 + 1; v3 < n; ++v3)
              {
                if (v3 == v) continue;
                if (adj[u][v3] && adj[u2][v3] && adj[u3][v3])
                {
                  for (int u4 = u3 + 1; u4 < m; ++u4)
                  {
                    if (u4 == u) continue;
                    if (adj[u4][v] && adj[u4][v2] && adj[u4][v3])
                    {
                      for (int v4 = v3 + 1; v4 < n; ++v4)
                      {
                        if (v4 == v) continue;
                        if (adj[u][v4] && adj[u2][v4] && adj[u3][v4] && adj[u4][v4])
                        {
                          for (int v5 = v4 + 1; v5 < n; ++v5)
                          {
                            if (v5 == v) continue;
                            if (adj[u][v5] && adj[u2][v5] && adj[u3][v5] && adj[u4][v5])
                            {
                              for (int v6 = v5 + 1; v6 < n; ++v6)
                              {
                                if (v6 == v) continue;
                                if (adj[u][v6] && adj[u2][v6] && adj[u3][v6] && adj[u4][v6])
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
              }
            }
          }
        }
      }
    }
  }
  return false;
}

void K46Store::storeKst(const Graph& adj, int u, int v)
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
      for (int k = j + 1; k < u_cands_size; ++k)
      {
        v_cands_size = 0;

        for (int node_v = 0; node_v < adj.n; ++node_v)
        {
          if (node_v == v) continue;

          if (adj[u][node_v] && adj[u_cands[i]][node_v] && adj[u_cands[j]][node_v] &&
              adj[u_cands[k]][node_v])
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
                for (int e = d + 1; e < v_cands_size; ++e)
                {
                  Kst<S_, T_> created(
                      {u, u_cands[i], u_cands[j], u_cands[k]},
                      {v, v_cands[a], v_cands[b], v_cands[c], v_cands[d], v_cands[e]});

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
  }
}

void K46Store::reevalCircles(const Graph& adj)
{
  KstHelper::reevalCircles<S_, T_>(circles_, edges_in_circles_, adj);
}

bool K46Store::reflipCircle(Graph& adj, Probabilities& prob)
{
  return KstHelper::reflipCircle<S_, T_>(circles_, edges_in_circles_, adj, prob);
}
