#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string_view>

namespace Constants
{
inline constexpr int MAX_SIZE = 40;
inline constexpr int HISTOGRAM_SIZE = 5;
inline constexpr int MAX_GRAPHS_TO_SAVE = 5;
inline constexpr double PROBABILITY_MULTIPLIER = 1.0;
inline constexpr std::string_view OUTPUT_DIRECTORY = "output";
inline constexpr std::string_view CONFIG_FILE = "config.conf";
inline constexpr std::string_view RESULTS_FILE = "results";
inline constexpr std::string_view LOG_FILE = "log";
inline constexpr std::string_view RUN_STATS_LOCATION = "runstats/runstats.txt";
}  // namespace Constants

#endif  // CONSTANTS_H