#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>

#include "Logs.h"

class Logger
{
 private:
  std::ofstream logFile_;

 public:
  Logger(const std::string& filename);
  void log(const Logs& logs);
  ~Logger();
};

#endif  // LOGGER_H
