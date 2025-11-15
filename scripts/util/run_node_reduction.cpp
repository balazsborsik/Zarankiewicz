#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>

#include "config/Constants.h"
#include "core/Graph.h"
#include "structure/KstStore.h"
#include "util/Util.h"

// g++ run_node_reduction.cpp ../../src/core/Graph.cpp ../../src/util/Util.cpp
// ../../src/structure/*.cpp -I../../include -o run_node_reduction

inline constexpr int MAXGRAPHSINCACHE = 10;

struct GraphRowColumn
{
  Graph graph;
  int rowSum[Constants::MAX_SIZE];
  int colSum[Constants::MAX_SIZE];
  int minRowIdx = 0;
  int minColIdx = 0;

  // Default constructor so arrays of GraphRowColumn can be value-initialized.
  GraphRowColumn() : graph(Constants::MAX_SIZE, Constants::MAX_SIZE) {}

  void remove_n()
  {
    --graph.n;  // n is now n-1
    graph.edges -= colSum[minColIdx];
    if (minColIdx != graph.n)
    {  // because n is now n-1
      for (int i = 0; i < graph.m; ++i)
      {
        std::swap(graph.adj[i][minColIdx], graph.adj[i][graph.n]);
      }
      std::swap(colSum[minColIdx], colSum[graph.n]);
    }

    colSum[graph.n] = Constants::MAX_SIZE;
    minColIdx = 0;
    for (int i = 0; i < Constants::MAX_SIZE; ++i)
    {
      if (graph.adj[i][graph.n] == 1)
      {
        --rowSum[i];
      }
      if (rowSum[i] < rowSum[minRowIdx])
      {
        minRowIdx = i;
      }
      if (colSum[i] < colSum[minColIdx])
      {
        minColIdx = i;
      }
    }
  }

  void remove_m()
  {
    --graph.m;  // m is now m-1
    graph.edges -= rowSum[minRowIdx];
    if (minRowIdx != graph.m)
    {  // because m is now m-1
      for (int j = 0; j < graph.n; ++j)
      {
        std::swap(graph.adj[minRowIdx][j], graph.adj[graph.m][j]);
      }
      std::swap(rowSum[minRowIdx], rowSum[graph.m]);
    }

    rowSum[graph.m] = Constants::MAX_SIZE;
    minRowIdx = 0;
    for (int j = 0; j < Constants::MAX_SIZE; ++j)
    {
      if (graph.adj[graph.m][j] == 1)
      {
        --colSum[j];
      }
      if (rowSum[j] < rowSum[minRowIdx])
      {
        minRowIdx = j;
      }
      if (colSum[j] < colSum[minColIdx])
      {
        minColIdx = j;
      }
    }
  }
  const int getPotential() const { return graph.edges - (rowSum[minRowIdx] + colSum[minColIdx]); }

  GraphRowColumn(int m, int n) : graph(m, n) {}
};
inline bool operator<(const GraphRowColumn& lhs, const GraphRowColumn& rhs)
{
  if (lhs.graph.edges == rhs.graph.edges)
  {
    return lhs.getPotential() < rhs.getPotential();
  }
  return lhs.graph.edges < rhs.graph.edges;
}
inline bool operator>(const GraphRowColumn& lhs, const GraphRowColumn& rhs)
{
  return rhs < lhs;
}
inline bool operator<=(const GraphRowColumn& lhs, const GraphRowColumn& rhs)
{
  return !(lhs > rhs);
}
inline bool operator>=(const GraphRowColumn& lhs, const GraphRowColumn& rhs)
{
  return !(lhs < rhs);
}

std::unique_ptr<KstStore> kstStore;
std::vector<std::vector<std::array<GraphRowColumn, MAXGRAPHSINCACHE>>> graphsCache(
    Constants::MAX_SIZE + 1,
    std::vector<std::array<GraphRowColumn, MAXGRAPHSINCACHE>>(
        Constants::MAX_SIZE + 1, std::array<GraphRowColumn, MAXGRAPHSINCACHE>()));

void addToGraphsCache(const GraphRowColumn& grc, int m, int n)
{
  auto& arr = graphsCache[m][n];
  if (arr.back() < grc)
  {
    arr.back() = std::move(grc);
    int i = arr.size() - 1;
    while (i > 0 && arr[i - 1] < arr[i])
    {
      std::swap(arr[i - 1], arr[i]);
      --i;
    }
  }
}

GraphRowColumn create_from_file(int m, int n, const std::string& filename)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("Failed to open file: " + filename);
  }

  GraphRowColumn graphRowColumn(m, n);
  Graph& graph = graphRowColumn.graph;
  int edge;
  int columnSum[Constants::MAX_SIZE] = {};
  int rowSum[Constants::MAX_SIZE] = {};
  std::fill_n(columnSum + n, Constants::MAX_SIZE - n, Constants::MAX_SIZE);
  std::fill_n(rowSum + m, Constants::MAX_SIZE - m, Constants::MAX_SIZE);
  for (int i = 0; i < m; ++i)
  {
    for (int j = 0; j < n; ++j)
    {
      file >> edge;
      if (edge)
      {
        graph.addEdge(i, j);
        ++rowSum[i];
        ++columnSum[j];
      }
    }
  }
  file.close();
  for (int i = 0; i < Constants::MAX_SIZE; ++i)
  {
    graphRowColumn.colSum[i] = columnSum[i];
    graphRowColumn.rowSum[i] = rowSum[i];
    if (rowSum[i] < rowSum[graphRowColumn.minRowIdx])
    {
      graphRowColumn.minRowIdx = i;
    }
    if (columnSum[i] < columnSum[graphRowColumn.minColIdx])
    {
      graphRowColumn.minColIdx = i;
    }
  }
  return graphRowColumn;
}

void readAllGraphs(std::string directory)
{
  try
  {
    std::regex re(R"(Z(\d+)_(\d+)_(\d+)_(\d+)_(\d+)\.txt)");
    for (const auto& entry : std::filesystem::directory_iterator(directory))
    {
      std::smatch match;
      std::string filename = entry.path().string();
      if (std::regex_search(filename, match, re))
      {
        int m = std::stoi(match[1]);
        int n = std::stoi(match[2]);
        if (kstStore->getS() != std::stoi(match[3]) || kstStore->getT() != std::stoi(match[4]))
          throw std::runtime_error("KstStore s and t do not match file s and t");
        int edgeCount = std::stoi(match[5]);

        addToGraphsCache(create_from_file(m, n, filename), m, n);
      }
    }
  }
  catch (const std::filesystem::filesystem_error& e)
  {
    std::cerr << "Error: " << e.what() << "\n";
  }
}

void runIteration()
{
  for (int m_i = Constants::MAX_SIZE; m_i >= 3; --m_i)
  {
    for (int n_i = Constants::MAX_SIZE; n_i >= 3; --n_i)
    {
      for (int i = 0; i < MAXGRAPHSINCACHE && graphsCache[m_i][n_i][i].graph.edges != 0; ++i)
      {
        GraphRowColumn graphRowColumn_n = graphsCache[m_i][n_i][i];
        GraphRowColumn graphRowColumn_m = graphsCache[m_i][n_i][i];
        graphRowColumn_m.remove_m();
        graphRowColumn_n.remove_n();
        addToGraphsCache(graphRowColumn_m, m_i - 1, n_i);
        addToGraphsCache(graphRowColumn_n, m_i, n_i - 1);
      }
    }
  }
}

void printTop2Graphs(std::string directory)
{
  for (int m_i = 2; m_i <= Constants::MAX_SIZE; ++m_i)
  {
    for (int n_i = 2; n_i <= Constants::MAX_SIZE; ++n_i)
    {
      for (int i = 0; i < 2 && graphsCache[m_i][n_i][i].graph.edges != 0; ++i)
      {
        const Graph& graph = graphsCache[m_i][n_i][i].graph;
        std::ostringstream filename;
        filename << directory << "/Z" << m_i << "_" << n_i << "_" << kstStore->getS() << "_"
                 << kstStore->getT() << "_" << graph.edges << ".txt";
        std::ofstream outFile(filename.str());
        if (!outFile.is_open())
        {
          throw std::runtime_error("Failed to open file for writing: " + filename.str());
        }
        for (int r = 0; r < graph.m; ++r)
        {
          for (int c = 0; c < graph.n; ++c)
          {
            outFile << graph.adj[r][c] << ' ';
          }
          outFile << '\n';
        }
        outFile.close();
      }
    }
  }
}

bool is_number(const std::string& s)
{
  std::string::const_iterator it = s.begin();
  while (it != s.end() && std::isdigit(*it)) ++it;
  return !s.empty() && it == s.end();
}

int main(int argc, char* argv[])
{
  if (argc != 4 || !is_number(argv[2]) || !is_number(argv[3]))
  {
    std::cerr << "Usage: " << argv[0] << " <directory> <s> <t>\n";
    return 1;
  }
  int s = std::stoi(argv[2]);
  int t = std::stoi(argv[3]);
  if (s > t || s < 2 || t > 6)
  {
    std::cerr << "Usage: " << argv[0] << " <directory> <s> <t>\nWhere 2 <= s <= t <= 6\n";
    return 2;
  }
  kstStore = Util::createKstStore(s, t);
  readAllGraphs(argv[1]);
  runIteration();
  printTop2Graphs(argv[1]);
}