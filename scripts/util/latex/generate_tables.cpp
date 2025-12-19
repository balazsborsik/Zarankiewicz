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
        outfile << " | [" << val << "](https://github.com/balazsborsik/Zarankiewicz/blob/main/output/K" << s << t << "/graphs/Z" << i << '_' << j
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
\begin{tabular}{r)";

  for (int i = min; i <= max; ++i)
  {
    if ((i - min) % 5 == 0) ss << "|";
    ss << "c";
  }
  ss << R"(} 
$m \backslash n$)";
  for (int i = min; i <= max; ++i)
  {
    ss << " & " << i;
  }
  ss << R"( \\)";
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

bool isThereValueToCompare(int s, int t)
{
  std::string path =
      "./best_known_results/K" + std::to_string(s) + std::to_string(t) + "/merged_results.txt";
  std::ifstream file(path);
  return file.good();
}

bool isThereFixValueToCompare(int s, int t)
{
  std::string path =
      "./best_known_results/K" + std::to_string(s) + std::to_string(t) + "/merged_fix_results.txt";
  std::ifstream file(path);
  return file.good();
}

std::vector<std::vector<int>> getBestUpperBounds(int s, int t)
{
  std::vector<std::vector<int>> bestKnown;
  std::string path =
      "./best_known_results/K" + std::to_string(s) + std::to_string(t) + "/merged_results.txt";
  bestKnown = readMatrix(path);
  return bestKnown;
}

std::vector<std::vector<int>> getBestFixValues(int s, int t)
{
  std::vector<std::vector<int>> bestKnown;
  std::string path =
      "./best_known_results/K" + std::to_string(s) + std::to_string(t) + "/merged_fix_results.txt";
  bestKnown = readMatrix(path);
  return bestKnown;
}

std::string ismert_es_beallitjuk(int value)
{
  return "\\textbf{" + std::to_string(value) + "}";
  // return "\\textbf{" + std::to_string(value) + "}";
}

std::string ismert_es_nem_allitjuk_be(int value)
{
  return "\\underline{" + std::to_string(value) + "}";
}

std::string nem_ismert_es_beallitjuk(int value)
{
  return "\\fixedbox{\\textbf{" + std::to_string(value) + "}}";
}

std::string nem_ismert_es_nem_allitjuk_be(int value)
{
  return std::to_string(value);
}

std::string hiba_megjavitjuk(int value)
{
  return "$" + std::to_string(value) + "^*$";
  // return "\\underline{\\textbf{" + std::to_string(value) + "}}";
}

int print_in_latexformat(std::string output, const std::vector<std::vector<int>>& res, int max,
                         int s, int t, bool enable_comparison)
{
  std::vector<std::vector<std::string>> results;
  int total_count = 0;
  if (enable_comparison && isThereValueToCompare(s, t))
  {
    std::vector<std::vector<int>> upper_bounds = getBestUpperBounds(s, t);
    std::vector<std::vector<int>> fix_known;
    bool is_fixValue_there = isThereFixValueToCompare(s, t);
    if (is_fixValue_there) fix_known = getBestFixValues(s, t);
    results.resize(res.size(), std::vector<std::string>(res[0].size(), ""));
    int ismert_es_beallitjuk_count = 0;
    int ismert_es_nem_allitjuk_be_count = 0;
    int nem_ismert_es_beallitjuk_count = 0;
    int nem_ismert_es_nem_allitjuk_be_count = 0;
    int hiba_megjavitjuk_count = 0;

    for (size_t i = 0; i < res.size(); i++)
    {
      for (size_t j = 0; j < res[i].size(); j++)
      {
        if (res[i][j] > 0)
        {
          bool counts = (i >= s - 2 && j >= t - 2);
          if (counts) total_count++;
          if (is_fixValue_there && fix_known[i][j] > 0)
          {
            if (res[i][j] >= fix_known[i][j])
            {
              if (res[i][j] > fix_known[i][j])
              {
                results[i][j] = hiba_megjavitjuk(res[i][j]);
                if (counts) hiba_megjavitjuk_count++;
              }
              else
              {
                results[i][j] = ismert_es_beallitjuk(res[i][j]);
                if (counts) ismert_es_beallitjuk_count++;
              }
            }
            else
            {
              results[i][j] = ismert_es_nem_allitjuk_be(res[i][j]);
              if (counts) ismert_es_nem_allitjuk_be_count++;
            }
          }
          else
          {
            if (res[i][j] >= upper_bounds[i][j])
            {
              if (res[i][j] > upper_bounds[i][j])
              {
                results[i][j] = hiba_megjavitjuk(res[i][j]);
                if (counts) hiba_megjavitjuk_count++;
              }
              else
              {
                results[i][j] = nem_ismert_es_beallitjuk(res[i][j]);
                if (counts) nem_ismert_es_beallitjuk_count++;
              }
            }
            else
            {
              results[i][j] = nem_ismert_es_nem_allitjuk_be(res[i][j]);
              if (counts) nem_ismert_es_nem_allitjuk_be_count++;
            }
          }
        }
      }
    }
    std::cout << "K" << s << "," << t << " statistics:" << std::endl;
    std::cout << "Total count: " << total_count << std::endl;
    std::cout << "ismert_es_beallitjuk: " << ismert_es_beallitjuk_count << std::endl;
    std::cout << "ismert_es_nem_allitjuk_be: " << ismert_es_nem_allitjuk_be_count << std::endl;
    std::cout << "nem_ismert_es_beallitjuk: " << nem_ismert_es_beallitjuk_count << std::endl;
    std::cout << "nem_ismert_es_nem_allitjuk_be: " << nem_ismert_es_nem_allitjuk_be_count
              << std::endl;
    std::cout << "hiba_megjavitjuk: " << hiba_megjavitjuk_count << std::endl;
  }
  else
  {
    results.resize(res.size(), std::vector<std::string>(res[0].size(), ""));
    for (size_t i = 0; i < res.size(); i++)
    {
      for (size_t j = 0; j < res[i].size(); j++)
      {
        results[i][j] = std::to_string(res[i][j]);
      }
    }
  }

  std::ofstream outfile(output);

  std::string start = generate_latex_header(t, max);
  std::string end = generate_latex_footer(s, t);
  outfile << start << std::endl;
  for (int i = s; i <= max; i++)
  {
    if ((i - s) % 5 == 0) outfile << "\\hline\n";
    outfile << i;
    for (int j = t; j <= max; j++)
    {
      int val = res[i - 2][j - 2];
      std::string value_to_print = results[i - 2][j - 2];
      if (val)
        outfile << " & " << value_to_print;
      else
        outfile << " &  ";
    }
    outfile << " \\\\" << std::endl;
  }
  outfile << end;
  outfile.close();
  return total_count;
}

int main(int argc, char* argv[])
{
  int total_count = 0;
  for (int s = 2; s <= 6; s++)
  {
    for (int t = s; t <= 6; t++)
    {
      bool enable_comparison = true;

      std::string pathOfResults =
          "./../../../output/K" + std::to_string(s) + std::to_string(t) + "/current_results.txt";
      std::vector<std::vector<int>> res = readMatrix(pathOfResults);
      int maxSize = (s == 2 && t <= 3) ? 40 : 30;
      std::string latex =
          "./generated_tables/K" + std::to_string(s) + std::to_string(t) + "_results_latex.tex";
      std::string readme =
          "./readme_tables/K" + std::to_string(s) + std::to_string(t) + "_results_readme.md";
      total_count += print_in_latexformat(latex, res, maxSize, s, t, enable_comparison);
      print_in_readme_format(readme, res, maxSize, s, t);
    }
  }
  std::cout << total_count;

  return 0;
}