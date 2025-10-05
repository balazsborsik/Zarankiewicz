#include <chrono>
#include <fstream>

#include "config/Config.h"
#include "config/ConfigLoader.h"
#include "config/Constants.h"
#include "core/Runner.h"
#include "io/FileManager.h"
#include "logger/RunStats.h"

int main()
{
  srand(time(NULL));
  RunStats runStats;
  runStats.start();

  Runner runner;
  runner.run();

  runStats.save();
  getConfigInstance().runStats.save();

  return 0;
}