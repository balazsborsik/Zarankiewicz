#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string_view>

namespace Constants
{
inline constexpr int MAX_SIZE = 40;
inline constexpr int MAX_S = 6;
inline constexpr int MAX_T = 6;
inline constexpr int HISTOGRAM_SIZE = 5;
inline constexpr int MAX_GRAPHS_TO_SAVE = 5;
inline constexpr std::string_view OUTPUT_DIRECTORY = "output";
inline constexpr std::string_view GRAPHS_DIRECTORY = "graphs";
inline constexpr std::string_view LOGS_DIRECTORY = "logs";
inline constexpr std::string_view RESULTS_DIRECTORY = "results";
inline constexpr std::string_view CONFIG_FILE = "config.conf";
inline constexpr std::string_view RESULTS_FILE = "results";
inline constexpr std::string_view LOG_FILE = "log";
inline constexpr std::string_view RUNSTATS_FILE = "runstats";
inline constexpr std::string_view EXECUTION_TIME_FILE = "execution_time";
}  // namespace Constants

#endif  // CONSTANTS_H