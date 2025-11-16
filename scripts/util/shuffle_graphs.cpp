#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

// g++ shuffle_graphs.cpp -o shuffle_graphs
// shuffle_graphs <directory>

namespace fs = std::filesystem;

void shuffle_file_rows_and_columns(const fs::path& filepath)
{
  std::ifstream inFile(filepath, std::ios::binary);
  if (!inFile.is_open())
  {
    std::cerr << "Failed to open file: " << filepath << "\n";
    return;
  }

  std::vector<std::vector<std::string>> table;
  std::string line;
  bool hasTrailingNewline = false;

  // Read + split into columns
  while (std::getline(inFile, line))
  {
    std::stringstream ss(line);
    std::vector<std::string> row;
    std::string cell;

    while (ss >> cell)  // split by whitespace
      row.push_back(cell);

    if (!row.empty()) table.push_back(row);
  }

  // Detect trailing newline
  inFile.clear();
  inFile.seekg(0, std::ios::end);
  if (inFile.tellg() > 0)
  {
    inFile.seekg(-1, std::ios::end);
    char lastChar;
    inFile.get(lastChar);
    if (lastChar == '\n') hasTrailingNewline = true;
  }
  inFile.close();

  if (table.empty()) return;

  size_t colCount = table[0].size();
  for (const auto& row : table)
  {
    if (row.size() != colCount)
    {
      std::cerr << "Warning: inconsistent column sizes in: " << filepath << "\n";
      return;
    }
  }

  // Random generator
  std::random_device rd;
  std::mt19937 g(rd());

  // Shuffle columns (same for all rows)
  std::vector<size_t> colPerm(colCount);
  std::iota(colPerm.begin(), colPerm.end(), 0);
  std::shuffle(colPerm.begin(), colPerm.end(), g);

  std::vector<std::vector<std::string>> colShuffledTable = table;
  for (auto& row : colShuffledTable)
  {
    std::vector<std::string> newRow(colCount);
    for (size_t i = 0; i < colCount; i++) newRow[i] = row[colPerm[i]];
    row = std::move(newRow);
  }

  // Shuffle rows
  std::shuffle(colShuffledTable.begin(), colShuffledTable.end(), g);

  // Write back
  std::ofstream outFile(filepath, std::ios::binary | std::ios::trunc);
  if (!outFile.is_open())
  {
    std::cerr << "Failed to open file for writing: " << filepath << "\n";
    return;
  }

  for (size_t i = 0; i < colShuffledTable.size(); ++i)
  {
    for (size_t j = 0; j < colShuffledTable[i].size(); j++)
    {
      outFile << colShuffledTable[i][j];
      if (j + 1 < colShuffledTable[i].size()) outFile << ' ';
    }
    if (i < colShuffledTable.size() - 1 || hasTrailingNewline) outFile << '\n';
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
      shuffle_file_rows_and_columns(entry.path());
    }
  }

  std::cout << "Row+Column shuffling completed.\n";
  return 0;
}
