#ifndef CONFIG_H
#define CONFIG_H

#include <format>

#include "logger/Runstats.h"

struct Config
{
 public:
  RunStats runStats;
  int runCount;
  int iterations;
  int insideIterations;
  int s;
  int t;
  int min;
  int max;
  int probabilityType;  // 0: constant, 1: dynp
  int maxGraphsToSave;

  void loadConfig(const std::string& filename);
  std::string outputDirectory() const;
  std::string graphsDirectory() const;
  std::string finalResultsFileName() const;
  std::string resultsFileName(int id) const;
  std::string logFileName(int id) const;
  std::string outputFilename(int m, int n, int edgeCount) const;
  std::string executionTimeFilename() const;
  void saveExecutionTime() const;

 private:
  Config() = default;
  ~Config() = default;
  Config(const Config&) = delete;
  Config& operator=(const Config&) = delete;
  Config(Config&&) = delete;
  Config& operator=(Config&&) = delete;
  friend Config& getConfigInstance();
};

inline Config& getConfigInstance()
{
  static Config instance;
  return instance;
}

#endif  // CONFIG_H
