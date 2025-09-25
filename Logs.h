#ifndef LOGS_H
#define LOGS_H

#include <array>
#include <chrono>
#include <iostream>

#include "Constants.h"

class Logs
{
 private:
  int siz_ = 0;
  int maximum_ = 0;
  double mean_ = 0.0;
  double varianceAcc_ = 0.0;
  std::chrono::steady_clock::time_point start_;
  std::array<int, Constants::HISTOGRAM_SIZE> histogram{};

  double elapsedMilliseconds() const;

 public:
  void startTimer();

  void add(int x);

  double variance() const;

  void print(std::ostream& out = std::cout) const;
};

#endif  // LOGS_H
