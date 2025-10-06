#ifndef UTIL_H
#define UTIL_H

#include <algorithm>
#include <random>

class Util
{
 private:
  static std::mt19937 gen;

 public:
  static double randDouble();  // random double in (0,1)
  template <typename Iter>
  static void shuffle(Iter begin, Iter end)
  {
    std::shuffle(begin, end, gen);
  }
  static int upperBound(int m, int n, int s, int t);
  static int nCr(int n, int r);
};

#endif  // UTIL_H
