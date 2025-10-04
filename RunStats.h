#ifndef RUNSTATS_H
#define RUNSTATS_H

#include <chrono>
#include <fstream>

#include "ConfigLoader.h"
#include "Constants.h"

struct RunStats
{
  int id;
  int secondsRun;

  void start()
  {
    start(std::string(Constants::OUTPUT_DIRECTORY) + "/" + std::string(Constants::RUNSTATS_FILE) +
          ".txt");
  }

  void start(const std::string& filename)
  {
    start_ = std::chrono::steady_clock::now();
    filename_ = filename;
    ConfigLoader loader;
    loader.loadConfig(filename_);
    id = loader.getInt("id", -1);
    secondsRun = loader.getInt("secondsRun", -1);
    if (id == -1 || secondsRun == -1) throw std::runtime_error("Failed to load run stats");
  }

  void save()
  {
    std::ofstream file(filename_);
    if (!file.is_open()) throw std::runtime_error("Failed to open run stats file for writing");
    auto end = std::chrono::steady_clock::now();
    int seconds = static_cast<int>(std::chrono::duration<double>(end - start_).count());

    file << "id = " << id + 1 << "\n";
    file << "secondsRun = " << secondsRun + seconds << "\n";
  }

 private:
  std::chrono::steady_clock::time_point start_;
  std::string filename_;
};

#endif  // RUNSTATS_H
