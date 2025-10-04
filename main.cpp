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

int main()
{
  RunStats runStats;
  runStats.start();

  Runner runner;
  runner.run();

  runStats.save();
  getConfigInstance().runStats.save();

  return 0;
}