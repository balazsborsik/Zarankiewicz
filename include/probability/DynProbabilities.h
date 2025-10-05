#ifndef DYN_PROBABILITIES_H
#define DYN_PROBABILITIES_H

#include <algorithm>
#include <vector>

#include "probability/Probabilities.h"
#include "util/Util.h"

class DynProbabilities : public Probabilities
{
 private:
  std::vector<int> edgenum_m_;
  std::vector<int> edgenum_n_;
  double expected_m_;
  double expected_n_;
  double expected_n_percent_;

 public:
  // Constructor with dimensions
  DynProbabilities(int n, int m, int s, int t) : Probabilities(m, n, s, t)
  {
    int upper_bound = Util::upperBound(m_, n_, s_, t_);
    expected_m_ = (double)upper_bound / m + 0.5;
    expected_n_ = (double)upper_bound / n + 0.5;
    expected_n_percent_ = (double)upper_bound / (n * m);

    edgenum_m_.assign(m, 0);
    edgenum_n_.assign(n, 0);
  }

  // Constructor from a Graph
  DynProbabilities(const Graph &graph, int s, int t) : Probabilities(graph, s, t)
  {
    int m = graph.m;
    int n = graph.n;
    int upper_bound = Util::upperBound(m_, n_, s_, t_);

    expected_m_ = (double)upper_bound / m + 0.5;
    expected_n_ = (double)upper_bound / n + 0.5;
    expected_n_percent_ = (double)upper_bound / (n * m);

    edgenum_m_.assign(m, 0);
    edgenum_n_.assign(n, 0);

    for (int i = 0; i < m; i++)
    {
      for (int j = 0; j < n; j++)
      {
        if (graph.adj[i][j])
        {  // Assuming Graph stores adjacency as adj[m][n]
          edgenum_m_[i]++;
          edgenum_n_[j]++;
        }
      }
    }
  }

  void init()
  {
    int upper_bound = Util::upperBound(m_, n_, s_, t_);
    expected_m_ = (double)upper_bound / m_ + 0.5;
    expected_n_ = (double)upper_bound / n_ + 0.5;
    expected_n_percent_ = (double)upper_bound / (n_ * m_);

    edgenum_m_.assign(m_, 0);
    edgenum_n_.assign(n_, 0);
  };

  void reInitialize(int m, int n, int s, int t) override
  {
    Probabilities::reInitialize(m, n, s, t);
    init();
  }

  void reInitailize(const Graph &graph, int s, int t) override
  {
    Probabilities::reInitailize(graph, s, t);
    int m = graph.m;
    int n = graph.n;

    edgenum_m_.assign(m, 0);
    edgenum_n_.assign(n, 0);

    for (int i = 0; i < m; i++)
    {
      for (int j = 0; j < n; j++)
      {
        if (graph.adj[i][j])
        {
          edgenum_m_[i]++;
          edgenum_n_[j]++;
        }
      }
    }
  }

  void clear() override
  {
    std::fill(edgenum_m_.begin(), edgenum_m_.end(), 0);
    std::fill(edgenum_n_.begin(), edgenum_n_.end(), 0);
  }

  double get_multiplier(int v_m) const
  {
    if (edgenum_m_[v_m] >= expected_m_)
    {
      if (edgenum_m_[v_m] == expected_m_) return 0.9;
      return 0.5;
    }
    return 6.7 - 5.6 * ((double)edgenum_m_[v_m] / expected_m_);
  }

  double get_p(int v_m, int v_n) override
  {
    int n = edgenum_n_[v_n];
    return std::min(
        get_multiplier(v_m) * (expected_n_percent_ * 1.1 - (n / expected_n_) * expected_n_percent_),
        0.8);
  }

  void delete_edge(int v_m, int v_n) override
  {
    edgenum_m_[v_m]--;
    edgenum_n_[v_n]--;
  }

  void add_edge(int v_m, int v_n) override
  {
    edgenum_m_[v_m]++;
    edgenum_n_[v_n]++;
  }
};

#endif  // DYN_PROBABILITIES_H
