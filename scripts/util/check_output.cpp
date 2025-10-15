#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>

#include "config/Constants.h"
#include "core/Graph.h"
#include "structure/KstStore.h"
#include "util/Util.h"
using namespace std;

// g++ check_output.cpp ../../src/core/Graph.cpp ../../src/util/Util.cpp ../../src/structure/*.cpp
// -I../../include -o check_output

std::unique_ptr<KstStore> kstStore = Util::createKstStore(2, 2);
vector<string> problematic_filenames;

bool loadGraphWithErrorChecks(string filename, int m, int n, Graph& graph)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    std::cerr << "Error: cannot open file " << filename << std::endl;
    return 1;
  }

  std::string line;
  int lineCount = 0;
  for (; std::getline(file, line); ++lineCount)
  {
    std::stringstream ss(line);
    int edge = 0;
    int colCount = 0;
    for (colCount; ss >> edge; ++colCount)
    {
      graph.adj[lineCount][colCount] = edge;
    }
    if (colCount != n)
    {
      std::cerr << "Error: line " << lineCount + 1 << " in file " << filename << " does not have "
                << n << " columns." << std::endl;
      return true;
    }
  }
  if (lineCount != m)
  {
    std::cerr << "Error: file " << filename << " does not have " << m << " lines." << std::endl;
    return true;
  }
  file.close();
  return false;
}

bool isProblematic(int m, int n, int edgeNum, string filename)
{
  Graph graph(m, n);
  if (loadGraphWithErrorChecks(filename, m, n, graph)) return true;

  int currentEdges = 0;
  for (int i = 0; i < m; i++)
  {
    for (int j = 0; j < n; j++)
    {
      if (graph[i][j])
      {
        currentEdges++;
        graph.adj[i][j] = 0;
        if (kstStore->createsKst(graph, i, j))
        {
          return true;
        }
        graph.adj[i][j] = 1;
      }
    }
  }
  if (currentEdges != edgeNum)
  {
    std::cerr << "Error: file " << filename << " has " << currentEdges << " edges instead of "
              << edgeNum << " edges." << std::endl;
    return true;
  }
  return false;
}

void evaluateAllGraphs(string directory)
{
  int filesEvaluated = 0;
  try
  {
    std::regex re(R"(Z(\d+)_(\d+)_(\d+)_(\d+)_(\d+)\.txt)");
    for (const auto& entry : std::filesystem::directory_iterator(directory))
    {
      std::smatch match;
      std::string filename = entry.path().string();
      if (std::regex_search(filename, match, re))
      {
        ++filesEvaluated;
        int m = std::stoi(match[1]);
        int n = std::stoi(match[2]);
        int s = std::stoi(match[3]);
        int t = std::stoi(match[4]);
        int edgeCount = std::stoi(match[5]);
        if (s != kstStore->getS() || t != kstStore->getT())
        {
          kstStore = Util::createKstStore(s, t);
        }
        if (isProblematic(m, n, edgeCount, filename)) problematic_filenames.emplace_back(filename);
      }
    }
  }
  catch (const std::filesystem::filesystem_error& e)
  {
    std::cerr << "Error: " << e.what() << "\n";
  }
  std::cout << "Files evaluated: " << filesEvaluated << std::endl;
}

bool is_number(const std::string& s)
{
  std::string::const_iterator it = s.begin();
  while (it != s.end() && std::isdigit(*it)) ++it;
  return !s.empty() && it == s.end();
}

int main(int argc, char* argv[])
{
  if (argc < 3 || argc > 5 || argc == 4 ||
      (argc == 5 && (!is_number(argv[3]) || !is_number(argv[4]))) ||
      ((string)argv[2] != "-l" && (string)argv[2] != "-d"))
  {
    cerr << "Usage: " << argv[0]
         << " <directory> <-l (to list) or -d (to delete)> [<min size> <max size>]" << endl;
    return 1;
  }
  int min = 2;
  int max = Constants::MAX_SIZE;
  if (argc == 5)
  {
    int min = stoi(argv[3]);
    int max = stoi(argv[4]);
    if (min > max || min < 2 || max > Constants::MAX_SIZE)
    {
      cerr << "third parameter can't be greater than fourth parameter and both parameters must be "
              "in the range of: "
           << 2 << "-" << Constants::MAX_SIZE << endl;
      return 2;
    }
  }
  string dir = argv[1];

  evaluateAllGraphs(dir);

  for (const auto& elm : problematic_filenames)
  {
    cout << elm << endl;
    if ((string)argv[2] == "-d")
    {
      if (std::remove(elm.c_str()) == 0)
      {
        std::cout << "File deleted successfully.\n";
      }
      else
      {
        std::perror("Error deleting file");
      }
    }
  }
  if (problematic_filenames.empty()) cout << "All the files in the given directory are c4 free";
  return 0;
}