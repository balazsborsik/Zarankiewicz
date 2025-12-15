#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <regex>
#include <sstream>
#include <vector>

#include "config/Constants.h"
#include "core/Graph.h"
#include "structure/KstStore.h"
#include "util/Util.h"

using namespace std;

// g++ -O3 benchmark.cpp ../../../src/core/Graph.cpp ../../../src/util/Util.cpp
// ../../../src/structure/*.cpp ../../../src/structure/legacy/*.cpp -I../../../include -o benchmark

// benchmark ./../../../output

struct CachedGraph
{
  string filename;
  int m, n;
  Graph graph;

  CachedGraph(string fname, int rows, int cols)
      : filename(fname), m(rows), n(cols), graph(rows, cols)
  {
  }
};

std::unique_ptr<KstStore> kstStore;
vector<CachedGraph> graphCache;

void addRandomNoise(double percentage)
{
  static std::mt19937 rng(42);

  int offset = 0;
  if (percentage == 0.01) offset = 1;
  int totalEdgesAdded = 0;

  for (auto& entry : graphCache)
  {
    long long capacity = (long long)entry.m * entry.n;
    int targetEdges = static_cast<int>(capacity * percentage) + offset;

    if (targetEdges == 0) continue;

    std::uniform_int_distribution<int> distRow(0, entry.m - 1);
    std::uniform_int_distribution<int> distCol(0, entry.n - 1);

    int addedForGraph = 0;
    int attempts = 0;

    int maxAttempts = targetEdges * 50;

    while (addedForGraph < targetEdges && attempts < maxAttempts)
    {
      attempts++;
      int u = distRow(rng);
      int v = distCol(rng);

      if (entry.graph.adj[u][v] == 0)
      {
        entry.graph.adj[u][v] = -1;
        addedForGraph++;
        totalEdgesAdded++;
      }
    }
  }
  std::cout << "Noise added: " << totalEdgesAdded << " edges (" << (percentage * 100)
            << "%) across all graphs." << std::endl;
}

bool loadGraphData(string filename, int m, int n, Graph& graph)
{
  std::ifstream file(filename);
  if (!file.is_open()) return false;

  std::string line;
  int lineCount = 0;
  for (; std::getline(file, line); ++lineCount)
  {
    std::stringstream ss(line);
    int edge = 0;
    int colCount = 0;
    for (; ss >> edge; ++colCount)
    {
      graph.adj[lineCount][colCount] = edge;
    }
  }
  file.close();
  return true;
}

void cacheAllGraphs(string directory)
{
  graphCache.clear();
  cout << "Caching graphs from: " << directory << endl;
  int filesCached = 0;
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

        CachedGraph currentEntry(filename, m, n);

        if (loadGraphData(filename, m, n, currentEntry.graph))
        {
          graphCache.push_back(currentEntry);
          filesCached++;
        }
      }
    }
  }
  catch (const std::filesystem::filesystem_error& e)
  {
    std::cerr << "Error accessing directory: " << e.what() << "\n";
  }
  std::cout << "Caching complete. Total graphs in RAM: " << filesCached << std::endl;
}

void runBenchmarkStore(int s, int t, std::ofstream& outfile)
{
  vector<Graph> edgesToAdd;
  for (auto& entry : graphCache)
  {
    Graph graph(Constants::MAX_SIZE, Constants::MAX_SIZE);
    for (int i = 0; i < entry.m; i++)
    {
      for (int j = 0; j < entry.n; j++)
      {
        if (entry.graph.adj[i][j] == -1)
        {
          entry.graph.adj[i][j] = 0;
          graph.adj[i][j] = 1;
        }
      }
    }
    edgesToAdd.push_back(graph);
  }

  long long hits = 0;
  long long totalChecks = 0;
  auto start = std::chrono::high_resolution_clock::now();
  for (auto& entry : graphCache)
  {
    for (int i = 0; i < entry.m; i++)
    {
      for (int j = 0; j < entry.n; j++)
      {
        if (edgesToAdd[&entry - &graphCache[0]].adj[i][j])
        {
          entry.graph.adj[i][j] = 0;
          // kstStore->storeKst(entry.graph, i, j);
          kstStore->createsKst(entry.graph, i, j);
          totalChecks++;
          entry.graph.adj[i][j] = 1;
        }
      }
    }
    kstStore->clear();
  }

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> diff = end - start;

  outfile << "K" << s << t << " = " << diff.count() << " milliseconds" << endl;
  cout << "K" << s << t << " finished: " << diff.count() << " ms" << endl;
  cout << "Total checks: " << totalChecks << ", Hits: " << hits << endl;
};

void runBenchmark(int s, int t, std::ofstream& outfile)
{
  long long hits = 0;
  long long totalChecks = 0;

  auto start = std::chrono::high_resolution_clock::now();
  for (auto& entry : graphCache)
  {
    for (int i = 0; i < entry.m; i++)
    {
      for (int j = 0; j < entry.n; j++)
      {
        if (entry.graph.adj[i][j])
        {
          entry.graph.adj[i][j] = 0;
          if (kstStore->createsKst(entry.graph, i, j))
          {
            hits++;
          }

          totalChecks++;
          entry.graph.adj[i][j] = 1;
        }
      }
    }
    kstStore->clear();
  }

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> diff = end - start;

  outfile << "K" << s << t << " = " << diff.count() << " milliseconds" << endl;
  cout << "K" << s << t << " finished: " << diff.count() << " ms" << endl;
  cout << "Total checks: " << totalChecks << ", Hits: " << hits << endl;
}

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    cerr << "Usage: " << argv[0] << " <directory_path>" << endl;
    return 1;
  }

  string dir = argv[1];
  std::string outputFilename = "newhardcoded/benchmark_creates_with_exit_10_percent.txt";

  std::ofstream outfile(outputFilename);
  if (!outfile.is_open())
  {
    cerr << "Error: Could not open output file " << outputFilename << endl;
    return 1;
  }

  cout << "Starting Benchmarks..." << endl;

  for (int i = 2; i <= 6; i++)
  {
    for (int j = i; j <= 6; j++)
    {
      // if (j != i || i == 6) continue;
      kstStore = Util::createKstStore(i + 0, j + 0);

      cacheAllGraphs(dir + "/K" + to_string(i) + to_string(j) + "/graphs");
      if (graphCache.empty())
      {
        cerr << "No valid graphs found in directory." << endl;
        return 1;
      }
      addRandomNoise(0.1);
      runBenchmarkStore(i, j, outfile);
    }
  }

  outfile.close();
  cout << "Results saved to " << outputFilename << endl;

  outputFilename = "generic/benchmark_creates_with_exit_10_percent.txt";

  std::ofstream outfile2(outputFilename);
  if (!outfile2.is_open())
  {
    cerr << "Error: Could not open output file " << outputFilename << endl;
    return 1;
  }

  cout << "Starting Benchmarks..." << endl;

  for (int i = 2; i <= 6; i++)
  {
    for (int j = i; j <= 6; j++)
    {
      // if (j != i || i == 6) continue;
      kstStore = Util::createKstStore(i + 10, j + 10);

      cacheAllGraphs(dir + "/K" + to_string(i) + to_string(j) + "/graphs");
      if (graphCache.empty())
      {
        cerr << "No valid graphs found in directory." << endl;
        return 1;
      }
      addRandomNoise(0.1);
      runBenchmarkStore(i, j, outfile2);
    }
  }

  outfile2.close();
  cout << "Results saved to " << outputFilename << endl;

  outputFilename = "oldhardcoded/benchmark_creates_with_exit_10_percent.txt";

  std::ofstream outfile3(outputFilename);
  if (!outfile3.is_open())
  {
    cerr << "Error: Could not open output file " << outputFilename << endl;
    return 1;
  }

  cout << "Starting Benchmarks..." << endl;

  for (int i = 2; i <= 6; i++)
  {
    for (int j = i; j <= 6; j++)
    {
      // if (j != i || i == 6) continue;
      kstStore = Util::createKstStore(i + 20, j + 20);

      cacheAllGraphs(dir + "/K" + to_string(i) + to_string(j) + "/graphs");
      if (graphCache.empty())
      {
        cerr << "No valid graphs found in directory." << endl;
        return 1;
      }
      addRandomNoise(0.1);
      runBenchmarkStore(i, j, outfile3);
    }
  }

  outfile3.close();
  cout << "Results saved to " << outputFilename << endl;

  return 0;
}