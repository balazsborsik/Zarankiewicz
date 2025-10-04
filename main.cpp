#include <chrono>
#include <fstream>

#include "Config.h"
#include "ConfigLoader.h"
#include "Constants.h"
#include "FileManager.h"
#include "Runner.h"

struct RunStats
{
  int id;
  int secondsRun;
};

RunStats LoadRunStats()
{
  ConfigLoader loader;
  loader.loadConfig(std::string(Constants::RUN_STATS_LOCATION));
  RunStats stats;
  stats.id = loader.getInt("id", -1);
  stats.secondsRun = loader.getInt("secondsRun", -1);
  if (stats.id == -1 || stats.secondsRun == -1)
    throw std::runtime_error("Failed to load run stats");
  return stats;
}

void WriteRunStats(const RunStats& stats)
{
  std::ofstream file(std::string(Constants::RUN_STATS_LOCATION));
  if (!file.is_open()) throw std::runtime_error("Failed to open run stats file for writing");

  file << "id = " << stats.id << "\n";
  file << "secondsRun = " << stats.secondsRun << "\n";
}

int main()
{
  auto start = std::chrono::steady_clock::now();

  RunStats stats = LoadRunStats();
  stats.id += 1;

  Runner runner;
  getConfigInstance().id = stats.id;
  runner.run();

  auto end = std::chrono::steady_clock::now();
  double seconds = std::chrono::duration<double>(end - start).count();

  stats.secondsRun += static_cast<int>(seconds);
  WriteRunStats(stats);

  return 0;
}