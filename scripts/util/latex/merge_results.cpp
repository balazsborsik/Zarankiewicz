#include <algorithm>
#include <climits>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ranges>
#include <stdexcept>
#include <string>
#include <vector>

namespace fs = std::filesystem;
using namespace std;

const int ROWS = 39;
const int COLS = 39;
using Matrix = vector<vector<int>>;

int nCr(int n, int r)
{
  if (r < 0 || n < 0 || r > n) throw std::invalid_argument("Invalid nCr arguments");
  long ans = 1;
  r = r > n - r ? n - r : r;
  int j = 1;
  for (; j <= r; j++, n--)
  {
    if (n % j == 0)
    {
      ans *= n / j;
    }
    else if (ans % j == 0)
    {
      ans = ans / j * n;
    }
    else
    {
      ans = (ans * n) / j;
    }
  }
  return ans;
}

int Roman(int m, int n, int s, int t)
{
  if (m > n)
  {
    std::swap(s, t);
    std::swap(m, n);
  }
  int p = s - 1;
  if (m < s || n < t) return m * n;
  double previous = std::numeric_limits<double>::max();
  double result = std::numeric_limits<double>::max() - 1e307;
  while (previous > result)
  {
    previous = result;
    result = ((double)(t - 1) / nCr(p, s - 1)) * nCr(m, s) + n * ((p + 1) * (s - 1)) / (double)s;
    p++;
  }
  double previous2 = std::numeric_limits<double>::max();
  if (s != t)
  {
    std::swap(s, t);
    std::swap(m, n);
    p = s - 1;
    double result2 = std::numeric_limits<double>::max() - 1e307;
    while (previous2 > result2)
    {
      previous2 = result2;
      result2 = ((double)(t - 1) / nCr(p, s - 1)) * nCr(m, s) + n * ((p + 1) * (s - 1)) / (double)s;
      p++;
    }
  }
  return previous < previous2 ? (int)previous : (int)previous2;
}

bool endsWith(const string& fullString, const string& ending)
{
  if (fullString.length() >= ending.length())
  {
    return (0 ==
            fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
  }
  return false;
}

Matrix readMatrix(const fs::path& filePath)
{
  Matrix mat(ROWS, vector<int>(COLS));
  ifstream inputFile(filePath);

  if (!inputFile.is_open())
  {
    throw runtime_error("Could not open file: " + filePath.string());
  }

  for (int i = 0; i < ROWS; ++i)
  {
    for (int j = 0; j < COLS; ++j)
    {
      inputFile >> mat[i][j];
    }
  }
  inputFile.close();
  return mat;
}

void writeMatrix(const fs::path& filePath, const Matrix& mat)
{
  ofstream outputFile(filePath);
  if (!outputFile.is_open())
  {
    throw runtime_error("Could not open file for writing: " + filePath.string());
  }

  for (int i = 0; i < ROWS; ++i)
  {
    for (int j = 0; j < COLS; ++j)
    {
      outputFile << mat[i][j] << (j == COLS - 1 ? "" : " ");
    }
    outputFile << "\n";
  }
  outputFile.close();
}

void mergeFiles(string targetDir, int s, int t)
{
  vector<fs::path> fixFiles;
  vector<fs::path> resultFiles;

  if (!fs::exists(targetDir))
  {
    throw runtime_error("Directory '" + targetDir + "' does not exist.");
  }

  for (const auto& entry : fs::directory_iterator(targetDir))
  {
    if (entry.is_regular_file())
    {
      string filename = entry.path().filename().string();

      if (endsWith(filename, "_fix_results.txt") && filename != "merged_fix_results.txt")
      {
        fixFiles.push_back(entry.path());
      }
      else if (endsWith(filename, "_results.txt") && !endsWith(filename, "_fix_results.txt") &&
               filename != "merged_results.txt")
      {
        resultFiles.push_back(entry.path());
      }
    }
  }
  for (auto& files : fixFiles)
  {
    string name = files.filename().string();
    string suffix = "_fix_results.txt";

    if (name.ends_with(suffix))
    {
      name.erase(name.size() - suffix.size());
    }
    else
    {
      throw runtime_error("Unexpected filename format: " + name + " in " + targetDir);
    }
    auto it = std::find_if(resultFiles.begin(), resultFiles.end(), [name](const fs::path& path)
                           { return path.filename().string().starts_with(name); });
    if (it == resultFiles.end())
    {
      throw runtime_error("No corresponding result file found for fix file: " + name + " in " +
                          targetDir);
    }
  }
  Matrix mergedFix(ROWS, vector<int>(COLS, 0));

  if (!fixFiles.empty())
  {
    cout << "Processing " << fixFiles.size() << " fix files... in " << targetDir << endl;

    for (const auto& path : fixFiles)
    {
      Matrix current = readMatrix(path);

      for (int i = 0; i < ROWS; ++i)
      {
        for (int j = 0; j < COLS; ++j)
        {
          int val = current[i][j];
          if (val != 0)
          {
            if (mergedFix[i][j] != 0)
            {
              if (mergedFix[i][j] != val)
              {
                throw runtime_error("Conflict detected in fix files: " + path.filename().string() +
                                    " in " + targetDir + " at position [" + to_string(i) + "][" +
                                    to_string(j) + "]. Values: " + to_string(mergedFix[i][j]) +
                                    " vs " + to_string(val));
              }
            }
            else
            {
              mergedFix[i][j] = val;
            }
          }
        }
      }
    }

    fs::path outputFixPath = fs::path(targetDir) / "merged_fix_results.txt";
    writeMatrix(outputFixPath, mergedFix);
    cout << "Created: " << outputFixPath.string() << endl;
  }
  else
  {
    cout << "No fix files found. Skipping fix merge." << endl;
  }

  Matrix mergedResults(ROWS, vector<int>(COLS, 0));

  if (!resultFiles.empty())
  {
    cout << "Processing " << resultFiles.size() << " result files..." << endl;

    vector<Matrix> allResults;
    for (const auto& path : resultFiles)
    {
      allResults.push_back(readMatrix(path));
    }

    for (int i = 0; i < ROWS; ++i)
    {
      for (int j = (s == t) ? i : 0; j < COLS; ++j)
      {
        int minVal = INT_MAX;
        bool valueFound = false;

        for (const auto& mat : allResults)
        {
          int val = mat[i][j];
          if (val != 0)
          {
            if (val < minVal)
            {
              minVal = val;
            }
            valueFound = true;
          }
        }

        // If we found any non-zero values, use the minimum. Otherwise Roman.
        int finalResultValue = valueFound ? minVal : Roman(i + 2, j + 2, s, t);
        mergedResults[i][j] = finalResultValue;

        // 4. Validation against Merged Fix
        // "if the value is non 0 in merged_fix_results, and if the value from results is less...
        // throw exception"
        int fixVal = mergedFix[i][j];

        // We only check validation if both have data.
        // If result is 0 (no data), we generally don't compare against fix unless specified
        // otherwise. Assuming standard logic: we validate only actual calculated values.
        if (fixVal != 0)
        {
          if (finalResultValue != fixVal)
          {
            throw runtime_error("Validation Failed at [" + to_string(i) + "][" + to_string(j) +
                                "]. Result value (" + to_string(finalResultValue) +
                                ") is not equal with Fix value (" + to_string(fixVal) + ").");
          }
        }
      }
    }

    // We write the merged results as well (implied by the request to process them)
    fs::path outputResultPath = fs::path(targetDir) / "merged_results.txt";
    writeMatrix(outputResultPath, mergedResults);
    cout << "Created: " << outputResultPath.string() << endl;
  }
  else
  {
    cout << "No result files found." << endl;
  }

  cout << "Processing completed successfully." << endl;
}

int main()
{
  for (int s = 2; s <= 6; s++)
  {
    for (int t = s; t <= 6; t++)
    {
      string targetDir = "./best_known_results/K" + to_string(s) + to_string(t) + "/";
      if (!fs::exists(targetDir))
      {
        cerr << "Directory does not exist: " << targetDir << endl;
        continue;
      }
      mergeFiles(targetDir, s, t);
    }
  }
  return 0;
}