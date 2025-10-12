#ifndef RUNSTATS_H
#define RUNSTATS_H

#include <chrono>
#include <fstream>

#include "config/ConfigLoader.h"
#include "config/Constants.h"

struct RunStats
{
  int id;
  int secondsRun;

  void start();  // tracks the global runstats
  void start(const std::string& filename);
  void save();
  int getElapsedTime() const;

 private:
  std::chrono::steady_clock::time_point start_;
  std::string filename_;
};

#endif  // RUNSTATS_H
