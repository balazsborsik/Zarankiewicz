#include "util/Util.h"

#include <algorithm>
#include <iostream>
#include <limits>
#include <stdexcept>

#include "structure/K22Store.h"
#include "structure/K23Store.h"
#include "structure/K33Store.h"
#include "structure/K44Store.h"
#include "structure/K55Store.h"
#include "structure/K66Store.h"
#include "structure/KstStore.h"

std::mt19937 Util::gen(std::random_device{}());

double Util::randDouble()
{
  return (gen() / static_cast<double>(gen.max()));
}

int Util::upperBound(int m, int n, int s, int t)
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

std::unique_ptr<KstStore> Util::createKstStore(int s, int t)
{
  if (s == 2 && t == 2)
  {
    return std::make_unique<K22Store>();
  }
  else if (s == 2 && t == 3)
  {
    return std::make_unique<K23Store>();
  }
  else if (s == 3 && t == 3)
  {
    return std::make_unique<K33Store>();
  }
  else if (s == 4 && t == 4)
  {
    return std::make_unique<K44Store>();
  }
  else if (s == 5 && t == 5)
  {
    return std::make_unique<K55Store>();
  }
  else if (s == 6 && t == 6)
  {
    return std::make_unique<K66Store>();
  }
  else
  {
    throw std::invalid_argument("Unsupported K_{s,t} store type");
  }
}
