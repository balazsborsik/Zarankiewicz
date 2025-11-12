#include <iostream>
#include <limits>

// g++ roman.cpp -o roman
// Usage: roman m n s t

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

int roman(int m, int n, int s, int t)
{
  if (m > n)
  {
    std::swap(s, t);
    std::swap(m, n);
  }
  int p = s - 1;
  if (m < s || n < t) return m * s;
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

int main(int argc, char** argv)
{
  if (argc != 5)
  {
    std::cerr << "Usage: roman m n s t" << std::endl;
    return 1;
  }
  int m = std::stoi(argv[1]);
  int n = std::stoi(argv[2]);
  int s = std::stoi(argv[3]);
  int t = std::stoi(argv[4]);
  int result = roman(m, n, s, t);
  std::cout << "Result: " << result << std::endl;
  return 0;
}