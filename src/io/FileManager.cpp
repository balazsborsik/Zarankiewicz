#include "io/FileManager.h"

#include <filesystem>
#include <regex>

#include "config/Constants.h"
#include "core/Graph.h"

std::array<Graph, Constants::MAX_GRAPHS_TO_SAVE>
    FileManager::existingGraphs_[Constants::MAX_SIZE + 1][Constants::MAX_SIZE + 1] = {};

Graph FileManager::loadTextWholeFile(const std::string& filename, int m,
                                     int n)  // TODO try this to measure performance improvement
{
  std::ifstream file(filename);
  if (!file.is_open()) throw std::runtime_error("Failed to open file: " + filename);

  // Read entire file into a string buffer
  std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

  std::istringstream iss(buffer);

  Graph g(m, n);
  for (int i = 0; i < m; ++i)
  {
    for (int j = 0; j < n; ++j)
    {
      iss >> g.adj[i][j];
    }
  }

  return g;
}

Graph FileManager::create_from_file(int m, int n, const std::string& filename)
{
  Graph graph(m, n);
  int edge;
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("Failed to open file: " + filename);
  }
  for (int i = 0; i < m; ++i)
  {
    for (int j = 0; j < n; ++j)
    {
      file >> edge;
      if (edge)
        graph.addEdge(i, j);  // TODO we can do this faster by reading directly into adj and setting
                              // from the filename
    }
  }
  file.close();
  return graph;
}

void FileManager::print_graph(const Graph& graph, const std::string& filename)
{
  if (filename.empty())
  {
    print_graph(graph, std::cout);
    return;
  }
  std::ofstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("Failed to open file: " + filename);
  }
  print_graph(graph, file);
  file.close();
}

void FileManager::print_graph(const Graph& graph, std::ostream& out)
{
  for (int i = 0; i < graph.m; ++i)
  {
    for (int j = 0; j < graph.n; ++j)
    {
      out << graph[i][j] << ' ';
    }
    out << '\n';
  }
}

void FileManager::loadExistingGraphs(const std::string& directory)
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
        int edgeCount = std::stoi(match[5]);

        auto& arr = existingGraphs_[m][n];
        if (arr.back().edges < edgeCount)
        {
          arr.back() = std::move(create_from_file(m, n, filename));
          int i = arr.size() - 1;
          while (i > 0 && arr[i - 1].edges < arr[i].edges)
          {
            std::swap(arr[i - 1], arr[i]);
            --i;
          }
        }
      }
    }
  }
  catch (const std::filesystem::filesystem_error& e)
  {
    std::cerr << "Error: " << e.what() << "\n";
  }
}

void FileManager::addGraphToExisting(const Graph& graph)
{
  int m = graph.m;
  int n = graph.n;
  auto& arr = existingGraphs_[m][n];
  if (arr.back().edges < graph.edges)
  {
    arr.back() = std::move(graph);
    int i = arr.size() - 1;
    while (i > 0 && arr[i - 1].edges < arr[i].edges)
    {
      std::swap(arr[i - 1], arr[i]);
      --i;
    }
  }
}

std::array<Graph, Constants::MAX_GRAPHS_TO_SAVE> FileManager::getExistingGraphs(int m, int n)
{
  return existingGraphs_[m][n];
}

void FileManager::init(const std::string& directory)
{
  loadExistingGraphs(directory);
}