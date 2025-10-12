#include "logger/RunStats.h"

void RunStats::start()
{
  start(std::string(Constants::OUTPUT_DIRECTORY) + "/" + std::string(Constants::RUNSTATS_FILE) +
        ".txt");
}

void RunStats::start(const std::string& filename)
{
  start_ = std::chrono::steady_clock::now();
  filename_ = filename;
  ConfigLoader loader;
  loader.loadConfig(filename_);
  id = loader.getInt("id", -1);
  secondsRun = loader.getInt("secondsRun", -1);
  if (id == -1 || secondsRun == -1) throw std::runtime_error("Failed to load run stats");
}

void RunStats::save()
{
  std::ofstream file(filename_);
  if (!file.is_open()) throw std::runtime_error("Failed to open run stats file for writing");

  int seconds = getElapsedTime();
  file << "id = " << id + 1 << "\n";
  file << "secondsRun = " << secondsRun + seconds << "\n";
}

int RunStats::getElapsedTime() const
{
  auto end = std::chrono::steady_clock::now();
  return static_cast<int>(std::chrono::duration<double>(end - start_).count());
}