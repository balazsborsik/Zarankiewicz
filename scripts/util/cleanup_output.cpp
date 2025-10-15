#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>

#include "config/Constants.h"

using namespace std;

// g++ cleanup_output.cpp -I../../include -o cleanup_output

vector<pair<string, int>> existingGraphs_[Constants::MAX_SIZE + 1][Constants::MAX_SIZE + 1] = {};
vector<string> problematic_filenames;

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
        int edgeCount = std::stoi(match[5]);

        auto& arr = existingGraphs_[m][n];
        if (arr.back().second < edgeCount)
        {
          if (arr.back().first != "") problematic_filenames.emplace_back(arr.back().first);
          arr.back() = {filename, edgeCount};
          int i = arr.size() - 1;
          while (i > 0 && arr[i - 1].second < arr[i].second)
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

bool is_number(const std::string& s)
{
  std::string::const_iterator it = s.begin();
  while (it != s.end() && std::isdigit(*it)) ++it;
  return !s.empty() && it == s.end();
}

int main(int argc, char* argv[])
{
  if (argc < 4 || argc > 6 || argc == 5 ||
      (argc == 6 && (!is_number(argv[4]) || !is_number(argv[5]))) ||
      ((string)argv[2] != "-l" && (string)argv[2] != "-d") || !is_number(argv[3]))
  {
    cerr
        << "Usage: " << argv[0]
        << " <directory> <-l (to list) or -d (to delete)> <maxGraphsToKeep> [<min size> <max size>]"
        << endl;
    return 1;
  }
  int min = 2;
  int max = Constants::MAX_SIZE;
  if (argc == 5)
  {
    int min = stoi(argv[4]);
    int max = stoi(argv[5]);
    if (min > max || min < 2 || max > Constants::MAX_SIZE)
    {
      cerr << "third parameter can't be greater than fourth parameter and both parameters must be "
              "in the range of: "
           << 2 << "-" << Constants::MAX_SIZE << endl;
      return 2;
    }
  }
  string dir = argv[1];
  for (int i = 0; i <= Constants::MAX_SIZE; ++i)
  {
    for (int j = 0; j <= Constants::MAX_SIZE; ++j)
    {
      existingGraphs_[i][j].resize(stoi(argv[3]));
    }
  }

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
  if (problematic_filenames.empty())
    cout << "There are no files to delete";
  else
    cout << problematic_filenames.size() << " files found to delete" << endl;
  return 0;
}