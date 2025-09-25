#include "FileManager.h"

#include <filesystem>
#include <regex>

#include "Constants.h"

std::map<std::pair<int, int>,
         std::array<std::pair<int, std::string>, Constants::MAX_GRAPHS_TO_SAVE>>
    FileManager::existingFilenames_;

std::ofstream FileManager::logFile_;

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
      iss >> g[i][j];
    }
  }

  return g;
}

Graph FileManager::create_from_file(int m, int n, const std::string& filename)
{
  Graph graph(m, n);
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("Failed to open file: " + filename);
  }
  for (int i = 0; i < m; ++i)
  {
    for (int j = 0; j < n; ++j)
    {
      file >> graph[i][j];
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

void FileManager::loadExistingFilenames()
{
  existingFilenames_.clear();
  try
  {
    std::regex re(R"(Z(\d+)_(\d+)_(\d+)_(\d+)_(\d+)\.txt)");
    for (const auto& entry : std::filesystem::directory_iterator(Constants::OUTPUT_DIRECTORY))
    {
      std::smatch match;
      std::string filename = entry.path().filename().string();
      if (std::regex_search(filename, match, re))
      {
        int m = std::stoi(match[1]);
        int n = std::stoi(match[2]);
        int edgeCount = std::stoi(match[5]);

        auto& arr = existingFilenames_[{m, n}];
        if (arr.back().first < edgeCount)
        {
          arr.back() = {edgeCount, filename};
          int i = arr.size() - 1;
          while (i > 0 && arr[i - 1].first < arr[i].first)
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

void FileManager::init()
{
  logFile_.open(std::string(Constants::LOG_FILE), std::ios::app);
  if (!logFile_.is_open())
  {
    throw std::runtime_error("Failed to open log file");
  }

  loadExistingFilenames();
}

void FileManager::log(const Logs& logs)
{
  logs.print(logFile_);
  logFile_ << std::endl;
  logFile_.flush();
}

void FileManager::closeLogFile()
{
  logFile_.close();
}
