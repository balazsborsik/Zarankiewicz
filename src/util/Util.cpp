#include "util/Util.h"

#include <algorithm>
#include <iostream>
#include <limits>
#include <stdexcept>

#include "structure/DynamicKst.h"
#include "structure/GenericKstStore.h"
#include "structure/K22Store.h"
#include "structure/K23Store.h"
#include "structure/K24Store.h"
#include "structure/K25Store.h"
#include "structure/K26Store.h"
#include "structure/K33Store.h"
#include "structure/K34Store.h"
#include "structure/K35Store.h"
#include "structure/K36Store.h"
#include "structure/K44Store.h"
#include "structure/K45Store.h"
#include "structure/K46Store.h"
#include "structure/K55Store.h"
#include "structure/K56Store.h"
#include "structure/K66Store.h"
#include "structure/KstStore.h"
#include "structure/legacy/OldK23Store.h"
#include "structure/legacy/OldK24Store.h"
#include "structure/legacy/OldK25Store.h"
#include "structure/legacy/OldK26Store.h"
#include "structure/legacy/OldK33Store.h"
#include "structure/legacy/OldK34Store.h"
#include "structure/legacy/OldK35Store.h"
#include "structure/legacy/OldK36Store.h"
#include "structure/legacy/OldK44Store.h"
#include "structure/legacy/OldK45Store.h"
#include "structure/legacy/OldK46Store.h"
#include "structure/legacy/OldK55Store.h"
#include "structure/legacy/OldK56Store.h"
#include "structure/legacy/OldK66Store.h"

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
  switch (s)
  {
    case 2:
      switch (t)
      {
        case 2:
          return std::make_unique<K22Store>();
        case 3:
          return std::make_unique<K23Store>();
        case 4:
          return std::make_unique<K24Store>();
        case 5:
          return std::make_unique<K25Store>();
        case 6:
          return std::make_unique<K26Store>();
      }
      break;

    case 3:
      switch (t)
      {
        case 3:
          return std::make_unique<K33Store>();
        case 4:
          return std::make_unique<K34Store>();
        case 5:
          return std::make_unique<K35Store>();
        case 6:
          return std::make_unique<K36Store>();
      }
      break;

    case 4:
      switch (t)
      {
        case 4:
          return std::make_unique<K44Store>();
        case 5:
          return std::make_unique<K45Store>();
        case 6:
          return std::make_unique<K46Store>();
      }
      break;

    case 5:
      switch (t)
      {
        case 5:
          return std::make_unique<K55Store>();
        case 6:
          return std::make_unique<K56Store>();
      }
      break;

    case 6:
      switch (t)
      {
        case 6:
          return std::make_unique<K66Store>();
      }
      break;
  }
  if (s > 20 && t > 20)
  {
    s -= 20;
    t -= 20;
    switch (s)
    {
      case 2:
        switch (t)
        {
          case 2:
            return std::make_unique<K22Store>();
          case 3:
            return std::make_unique<OldK23Store>();
          case 4:
            return std::make_unique<OldK24Store>();
          case 5:
            return std::make_unique<OldK25Store>();
          case 6:
            return std::make_unique<OldK26Store>();
        }
        break;
      case 3:
        switch (t)
        {
          case 3:
            return std::make_unique<OldK33Store>();
          case 4:
            return std::make_unique<OldK34Store>();
          case 5:
            return std::make_unique<OldK35Store>();
          case 6:
            return std::make_unique<OldK36Store>();
        }
        break;
      case 4:
        switch (t)
        {
          case 4:
            return std::make_unique<OldK44Store>();
          case 5:
            return std::make_unique<OldK45Store>();
          case 6:
            return std::make_unique<OldK46Store>();
        }
        break;
      case 5:
        switch (t)
        {
          case 5:
            return std::make_unique<OldK55Store>();
          case 6:
            return std::make_unique<OldK56Store>();
        }
        break;
      case 6:
        switch (t)
        {
          case 6:
            return std::make_unique<OldK66Store>();
        }
        break;
    }
  }
  s -= 10;
  t -= 10;
  return std::make_unique<GenericKstStore>(s, t);
  // throw std::invalid_argument("Unsupported K_{s,t} store type");
}
