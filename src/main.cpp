#include <chrono>
#include <fstream>

#include "config/Config.h"
#include "config/ConfigLoader.h"
#include "config/Constants.h"
#include "core/Runner.h"
#include "io/FileManager.h"
#include "logger/RunStats.h"

int main(const int argc, const char* argv[])
{
  RunStats runStats;

  Runner runner;
  if (argc > 1)
  {
    runStats.start("experiments/" + std::string(argv[1]) +
                   std::string(Constants::OUTPUT_DIRECTORY) + "/" +
                   std::string(Constants::RUNSTATS_FILE) + ".txt");
    runner.run(argv[1]);
  }
  else
  {
    runStats.start();
    runner.run();
  }
  runStats.save();
  getConfigInstance().runStats.save();
  getConfigInstance().saveExecutionTime();

  return 0;
}