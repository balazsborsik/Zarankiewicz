#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

// g++ shuffle_rows.cpp -o shuffle_rows
// shuffle_rows <directory>

namespace fs = std::filesystem;

void shuffle_file_rows(const fs::path& filepath)
{
  std::ifstream inFile(filepath, std::ios::binary);
  if (!inFile.is_open())
  {
    std::cerr << "Failed to open file: " << filepath << "\n";
    return;
  }

  std::vector<std::string> rows;
  std::string line;
  bool hasTrailingNewline = false;

  // Read lines
  while (std::getline(inFile, line))
  {
    rows.push_back(line);
  }

  // Detect if file ends with a newline
  inFile.clear();
  inFile.seekg(0, std::ios::end);
  if (inFile.tellg() > 0)
  {
    inFile.seekg(-1, std::ios::end);
    char lastChar;
    inFile.get(lastChar);
    if (lastChar == '\n')
    {
      hasTrailingNewline = true;
    }
  }
  inFile.close();

  // Shuffle rows
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(rows.begin(), rows.end(), g);

  // Write back
  std::ofstream outFile(filepath, std::ios::binary | std::ios::trunc);
  if (!outFile.is_open())
  {
    std::cerr << "Failed to open file for writing: " << filepath << "\n";
    return;
  }

  for (size_t i = 0; i < rows.size(); ++i)
  {
    outFile << rows[i];
    if (i < rows.size() - 1 || hasTrailingNewline)
    {
      outFile << '\n';
    }
  }
}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <directory>\n";
    return 1;
  }

  fs::path dirPath = argv[1];
  if (!fs::exists(dirPath) || !fs::is_directory(dirPath))
  {
    std::cerr << "Invalid directory: " << dirPath << "\n";
    return 1;
  }

  for (const auto& entry : fs::directory_iterator(dirPath))
  {
    if (entry.is_regular_file() && entry.path().extension() == ".txt")
    {
      shuffle_file_rows(entry.path());
    }
  }

  std::cout << "Shuffling completed.\n";
  return 0;
}
