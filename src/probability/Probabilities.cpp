#include "probability/Probabilities.h"

Probabilities::Probabilities(int m, int n, int s, int t)
{
  init(m, n, s, t);
}

Probabilities::Probabilities(const Graph& graph, int s, int t)
{
  init(graph.m, graph.n, s, t);
  initDegreesFromGraph(graph);
};

void Probabilities::reInitialize(int m, int n, int s, int t)
{
  init(m, n, s, t);
}

void Probabilities::reInitialize(const Graph& graph, int s, int t)
{
  init(graph.m, graph.n, s, t);
  initDegreesFromGraph(graph);
}

void Probabilities::clear()
{
  std::fill(degree_m_, degree_m_ + Constants::MAX_SIZE, 0);
  std::fill(degree_n_, degree_n_ + Constants::MAX_SIZE, 0);
};

void Probabilities::delete_edge(int v_m, int v_n)
{
  --degree_m_[v_m];
  --degree_n_[v_n];
  --edge_count_;
};

void Probabilities::add_edge(int v_m, int v_n)
{
  ++degree_m_[v_m];
  ++degree_n_[v_n];
  ++edge_count_;
};

void Probabilities::init(int m, int n, int s, int t)
{
  m_ = m;
  n_ = n;
  s_ = s;
  t_ = t;
  edge_count_ = 0;
  int upper_bound = Util::upperBound(m_, n_, s_, t_);
  expected_m_ = ((double)upper_bound / m_);
  expected_n_ = ((double)upper_bound / n_);
  expected_percent_ = (double)upper_bound / (n_ * m_);
  std::fill(degree_m_, degree_m_ + Constants::MAX_SIZE, 0);
  std::fill(degree_n_, degree_n_ + Constants::MAX_SIZE, 0);
}

void Probabilities::initDegreesFromGraph(const Graph& graph)
{
  for (int i = 0; i < m_; i++)
  {
    for (int j = 0; j < n_; j++)
    {
      if (graph.adj[i][j])
      {
        ++degree_m_[i];
        ++degree_n_[j];
        ++edge_count_;
      }
    }
  }
}
