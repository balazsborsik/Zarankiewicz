#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
/*
g++ -o get_results get_results.cpp
get_results output_directory [output_filename]
*/

// .\scripts\util\get_results output/K22/graphs experiments/current_results.txt

using namespace std;

using Results = std::array<std::array<int, 40 - 1>, 40 - 1>;

Results results_ = {};

void scanAllGraphs(string directory)
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

        results_[m - 2][n - 2] =
            (results_[m - 2][n - 2] < edgeCount) ? edgeCount : results_[m - 2][n - 2];
      }
    }
  }
  catch (const std::filesystem::filesystem_error& e)
  {
    std::cerr << "Error: " << e.what() << "\n";
  }
}

void printResults(const std::string& filename)
{
  std::ofstream outfile;
  outfile.open(filename);
  if (!outfile.is_open())
  {
    throw std::runtime_error("Could not open log file: " + filename);
  }
  size_t siz = results_.size();
  for (size_t i = 0; i < siz; ++i)
  {
    for (size_t j = 0; j < siz; ++j)
    {
      outfile << results_[i][j] << " ";
    }
    outfile << std::endl;
  }
}

int main(int argc, char* argv[])
{
  if (argc != 2 && argc != 3)
  {
    cerr << "Usage: " << argv[0] << " <directory>" << "[output_filename]" << endl;
    return 1;
  }

  string directory = argv[1];
  string output_filename = "current_results.txt";
  if (argc == 3)
  {
    output_filename = argv[2];
  }

  scanAllGraphs(directory);
  printResults(output_filename);

  return 0;
}
