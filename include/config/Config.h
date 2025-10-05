#ifndef CONFIG_H
#define CONFIG_H

#include <format>

#include "config/ConfigLoader.h"
#include "config/Constants.h"
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

  void loadConfig(const std::string& filename)
  {
    ConfigLoader loader;
    if (loader.loadConfig(filename))
    {
      runCount = loader.getInt("runCount", 1);
      iterations = loader.getInt("iterations", 1);
      insideIterations = loader.getInt("insideIterations", 1);
      s = loader.getInt("s", 2);
      t = loader.getInt("t", 2);
      min = loader.getInt("min", 2);
      max = loader.getInt("max", 40);
      probabilityType = loader.getInt("probabilityType", 0);
      maxGraphsToSave = loader.getInt("maxGraphsToSave", 3);
      runStats.start(outputDirectory() + std::string(Constants::RUNSTATS_FILE) + ".txt");
    }
    else
    {
      throw std::runtime_error("Failed to load config file: " + filename);
    }

    if (min < 2 || max < 2 || min > Constants::MAX_SIZE || max > Constants::MAX_SIZE || min > max)
      throw std::invalid_argument("Invalid min or max value in config");
    if (maxGraphsToSave < 1 || maxGraphsToSave > Constants::MAX_GRAPHS_TO_SAVE)
      throw std::invalid_argument("Invalid maxGraphsToSave value in config");
  }

  std::string outputDirectory() const
  {
    return std::format("{}/K{}{}/", Constants::OUTPUT_DIRECTORY, s, t);
  }

  std::string graphsDirectory() const
  {
    return std::format("{}{}/", outputDirectory(), Constants::GRAPHS_DIRECTORY);
  }

  std::string finalResultsFileName() const
  {
    return std::format("{}{}/final{}{}.txt", outputDirectory(), Constants::RESULTS_DIRECTORY,
                       Constants::RESULTS_FILE, runStats.id);
  }

  std::string resultsFileName(int id) const
  {
    return std::format("{}{}/{}{}_{}.txt", outputDirectory(), Constants::RESULTS_DIRECTORY,
                       Constants::RESULTS_FILE, runStats.id, id);
  }

  std::string logFileName(int id) const
  {
    return std::format("{}{}/{}{}_{}.txt", outputDirectory(), Constants::LOGS_DIRECTORY,
                       Constants::LOG_FILE, runStats.id, id);
  }

  std::string outputFilename(int m, int n, int edgeCount) const
  {
    return std::format("{}Z{}_{}_{}_{}_{}.txt", graphsDirectory(), m, n, s, t, edgeCount);
  }
};

// global accessor for Config instance (thread-safe in C++11 and later)
inline Config& getConfigInstance()
{
  static Config instance;
  return instance;
}

#endif  // CONFIG_H
