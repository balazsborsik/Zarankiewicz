#include "Util.h"

#include <algorithm>
#include <limits>
#include <stdexcept>

int Util::upperBound(int m, int n, int s, int t)
{
  int p = s - 1;
  if (m > n)
  {
    std::swap(s, t);
    std::swap(m, n);
  }
  if (m < s) return m * s;
  double previous = std::numeric_limits<double>::max();
  double result = std::numeric_limits<double>::max() - 1e307;
  while (previous > result)
  {
    previous = result;
    result = ((double)(t - 1) / nCr(p, s - 1)) * nCr(m, s) + n * ((p + 1) * (s - 1)) / (double)s;
    p++;
  }
  return (int)previous;
}

int Util::nCr(int n, int r)
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
