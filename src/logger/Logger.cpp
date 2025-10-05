#include "logger/Logger.h"

#include <stdexcept>

Logger::Logger(const std::string& filename)
{
  logFile_.open(filename, std::ios::out | std::ios::app);
  if (!logFile_.is_open())
  {
    throw std::runtime_error("Could not open log file: " + filename);
  }
}

void Logger::log(const Logs& logs)
{
  if (logFile_.is_open())
  {
    logs.print(logFile_);
  }
  logFile_ << std::endl;
  logFile_.flush();
}

Logger::~Logger()
{
  if (logFile_.is_open())
  {
    logFile_.close();
  }
}