#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

/*
g++ -o current_results current_results.cpp
current_results 2 40
*/

std::vector<std::vector<int>> readMatrix(const std::string& filename)
{
  std::vector<std::vector<int>> matrix;
  std::ifstream file(filename);
  std::string line;

  if (!file)
  {
    std::cerr << "Error opening file: " << filename << std::endl;
    exit(1);
  }

  while (getline(file, line))
  {
    std::stringstream ss(line);
    std::vector<int> row;
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

std::string generate_readme_header(int min, int max)
{
  std::stringstream ss;

  ss << "| m \\ n |";
  for (int i = min; i <= max; ++i)
  {
    ss << " " << i << " |";
  }
  ss << "\n|---|";
  for (int i = min; i <= max; ++i)
  {
    ss << "---|";
  }
  return ss.str();
}

void print_in_readme_format(std::string output, const std::vector<std::vector<int>>& res, int max,
                            int s, int t)
{
  std::string prefix = "output/K" + std::to_string(s) + std::to_string(t) + "/graphs/";
  std::string start = generate_readme_header(t, max);
  std::ofstream outfile(output);
  outfile << start << std::endl;
  for (int i = s; i <= max; i++)
  {
    outfile << "| **" << i << "**";
    for (int j = t; j <= max; j++)
    {
      int val = res[j - 2][i - 2];
      if (val)
        // outfile<<" | ["<<val<<"](results/output/"<<'Z'<<i+2<<'_'<<j+2<<"_2_2_"<<val<<".txt)";
        outfile << " | [" << val << "](output/K" << s << t << "/graphs/Z" << i << '_' << j
                << "_2_2_" << val << ".txt)";
      else
        outfile << " |  ";
    }
    outfile << " |" << std::endl;
  }
  outfile.close();
}

std::string generate_latex_header(int min, int max)
{
  std::stringstream ss;
  ss << R"(\begin{center}
\renewcommand{\arraystretch}{1.2}
\setlength{\tabcolsep}{3pt}
\scriptsize
\resizebox{\textwidth}{!}{
\begin{tabular}{r|*{)";
  ss << (max - 1 - (min - 2));
  ss << R"(}{c}} 
$m \backslash n$)";
  for (int i = min; i <= max; ++i)
  {
    ss << " & " << i;
  }
  ss << R"( \\
\hline)";
  return ss.str();
}

std::string generate_latex_footer(int s, int t)
{
  std::stringstream ss;
  ss << R"(\end{tabular}
}
\vspace{1em}
\captionof{table}{$Z_{)"
     << s << "," << t << R"(}(m,n)$ alsó becslések}
\end{center})";
  return ss.str();
}

void print_in_latexformat(std::string output, const std::vector<std::vector<int>>& res, int max,
                          int s, int t)
{
  std::ofstream outfile(output);
  std::string start = generate_latex_header(t, max);
  std::string end = generate_latex_footer(s, t);
  outfile << start << std::endl;
  for (int i = s; i <= max; i++)
  {
    outfile << i;
    for (int j = t; j <= max; j++)
    {
      int val = res[i - 2][j - 2];
      if (val)
        outfile << " & " << val;
      else
        outfile << " &  ";
    }
    outfile << " \\\\" << std::endl;
  }
  outfile << end;
  outfile.close();
}

int main(int argc, char* argv[])
{
  for (int s = 2; s <= 6; s++)
  {
    for (int t = s; t <= 6; t++)
    {
      std::string pathOfResults =
          "./../../../output/K" + std::to_string(s) + std::to_string(t) + "/current_results.txt";
      std::vector<std::vector<int>> res = readMatrix(pathOfResults);
      int maxSize = (s == 2 && t <= 3) ? 40 : 30;
      std::string latex =
          "./tables/K" + std::to_string(s) + std::to_string(t) + "_results_latex.tex";
      std::string readme =
          "./readme_tables/K" + std::to_string(s) + std::to_string(t) + "_results_readme.txt";
      print_in_latexformat(latex, res, maxSize, s, t);
      print_in_readme_format(readme, res, maxSize, s, t);
    }
  }

  return 0;
}