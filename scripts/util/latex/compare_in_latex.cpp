#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// g++ compare_in_latex.cpp -o compare_in_latex
// Usage: compare_in_latex <matrixA.txt> <matrixB.txt> [subsize] [topindex]

using namespace std;
using Matrix = vector<vector<int>>;

Matrix readMatrix(const string& filename)
{
  Matrix m;
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
    while (ss >> num) row.push_back(num);
    m.push_back(row);
  }
  return m;
}

Matrix extractSubmatrix(const Matrix& mat, int subsize, bool fromTop, int topIndex = 0)
{
  int N = mat.size();
  int start;

  if (fromTop)
  {
    // Top mode: submatrix ends at topIndex, so start = topIndex - subsize
    start = topIndex - subsize;
    if (start < 0) start = 0;
  }
  else
  {
    // Bottom mode: submatrix ends at bottom of matrix
    start = N - subsize;
  }

  Matrix r;
  for (int i = start; i < start + subsize; ++i)
  {
    r.push_back(vector<int>(mat[i].begin() + start, mat[i].begin() + start + subsize));
  }
  return r;
}

string colorForDiff(int diff, bool better)
{
  // 10 discrete shades
  if (diff >= 10) return better ? "green!100" : "red!100";

  // shade from !50 .. !95
  int intensity = 50 + (diff - 1) * 5;
  return (better ? "green!" : "red!") + to_string(intensity);
}

void printInLatex(const Matrix& A, const Matrix& B, int subsize, int startIdx)
{
  cout <<
      R"(\begin{center}
\renewcommand{\arraystretch}{1.2}
\setlength{\tabcolsep}{3pt}
\scriptsize
\resizebox{\textwidth}{!}{
)";

  cout << "\\begin{tabular}{r|" << string(subsize, 'c') << "}\n";
  cout << "$m \\backslash n$";

  for (int j = 0; j < subsize - 1; ++j) cout << " & " << (startIdx + j + 2);
  cout << " \\\\\n\\hline\n";

  for (int i = 0; i < subsize - 1; ++i)
  {
    cout << (startIdx + i + 2);
    for (int j = 0; j < subsize - 1; ++j)
    {
      int a = A[i][j], b = B[i][j];

      if (a == 0)
      {
        cout << " & ";
        continue;
      }

      if (a == b)
      {
        cout << " & " << a;
        continue;
      }

      int diff = abs(a - b);
      bool better = (a > b);
      string color = colorForDiff(diff, better);
      cout << " & \\textcolor{" << color << "}{" << a << "}";
    }
    cout << " \\\\\n";
  }
  cout << "\\end{tabular}\n";
  cout << "}\n\\end{center}\n";
}

int main(int argc, char* argv[])
{
  if (argc < 3 || argc > 5)
  {
    cerr << "Usage: " << argv[0] << " <matrixA.txt> <matrixB.txt> [subsize] [top_index]\n";
    cerr << "  subsize defaults to 40\n";
    cerr << "  top_index used only if you want top mode\n";
    return 1;
  }

  string fileA = argv[1];
  string fileB = argv[2];

  int subsize = 40;
  bool fromTop = false;
  int topIndex = 0;

  if (argc >= 4) subsize = stoi(argv[3]);
  if (argc == 5)
  {
    fromTop = true;
    topIndex = stoi(argv[4]);
    if (topIndex > 40) topIndex = 40;  // clamp
  }

  Matrix A = readMatrix(fileA);
  Matrix B = readMatrix(fileB);

  if (A.size() != B.size() || A.empty())
  {
    cerr << "Matrices must be same size and non-empty.\n";
    return 1;
  }

  int startIndex = fromTop ? (topIndex - subsize) : (A.size() - subsize);
  if (startIndex < 0) startIndex = 0;

  A = extractSubmatrix(A, subsize, fromTop, topIndex);
  B = extractSubmatrix(B, subsize, fromTop, topIndex);

  printInLatex(A, B, subsize, startIndex);
  return 0;
}