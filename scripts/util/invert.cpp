#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
/*
g++ -o invert invert.cpp
invert file.txt
*/

using namespace std;

// Function to read a matrix from a file
vector<vector<int>> readMatrix(const string& filename)
{
  vector<vector<int>> matrix;
  ifstream file(filename);
  string line;

  if (!file)
  {
    cerr << "Error opening file: " << filename << endl;
    exit(1);
  }

  while (getline(file, line))
  {
    stringstream ss(line);
    vector<int> row;
    int num;
    while (ss >> num)
    {
      row.push_back(num);
    }
    matrix.push_back(row);
  }

  file.close();
  return matrix;
}

void printMatrixInverted(const string& filename, vector<vector<int>>& matrix)
{

     std::ofstream outfile(filename, std::ios::trunc);
    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing.\n";
    }
    for (int j = 0; j < matrix[0].size(); ++j) {
        for (int i = 0; i < matrix.size(); ++i) {
            outfile << matrix[i][j] << " ";
        }
        outfile << "\n";
    }

    outfile.close();
}


int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    cerr << "Usage: " << argv[0] << " <input.txt>" << endl;
    return 1;
  }

  string file1 = argv[1];  // First input file

  vector<vector<int>> matrix1 = readMatrix(file1);
    printMatrixInverted(file1, matrix1);

  return 0;
}
