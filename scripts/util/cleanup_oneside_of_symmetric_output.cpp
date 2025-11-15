#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>

using namespace std;

// g++ cleanup_oneside_of_symmetric_output.cpp -o cleanup_oneside_of_symmetric_output
// cleanup_oneside_of_symmetric_output <dir> <-l (to list) or -d (to delete)>
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
        int m = std::stoi(match[1]);
        int n = std::stoi(match[2]);
        int s = std::stoi(match[3]);
        int t = std::stoi(match[4]);
        if (s != t)
        {
          std::cerr << "s, t in " << filename << " are asymmetric\n ";
          continue;
        }
        ++filesEvaluated;
        int edgeCount = std::stoi(match[5]);
        if (m > n)
        {
          problematic_filenames.emplace_back(filename);
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
  if (argc != 3 || ((string)argv[2] != "-l" && (string)argv[2] != "-d"))
  {
    cerr << "Usage: " << argv[0] << " <directory> <-l (to list) or -d (to delete)>" << endl;
    return 1;
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
  if (problematic_filenames.empty())
    cout << "There are no files to delete";
  else
    cout << problematic_filenames.size() << " files found to delete" << endl;
  return 0;
}