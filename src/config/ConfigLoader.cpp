#include "config/ConfigLoader.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

bool ConfigLoader::loadConfig(const std::string& filename)
{
  std::ifstream file(filename);
  if (!file.is_open()) return false;

  std::string line;
  while (std::getline(file, line))
  {
    // Remove comments (everything after //)
    auto commentPos = line.find("//");
    if (commentPos != std::string::npos) line = line.substr(0, commentPos);

    // Trim leading/trailing whitespace
    line.erase(line.begin(), std::find_if(line.begin(), line.end(),
                                          [](unsigned char ch) { return !std::isspace(ch); }));
    line.erase(
        std::find_if(line.rbegin(), line.rend(), [](unsigned char ch) { return !std::isspace(ch); })
            .base(),
        line.end());

    if (line.empty()) continue;

    // Find the '=' separator
    auto equalPos = line.find('=');
    if (equalPos == std::string::npos) continue;  // skip invalid lines

    std::string key = line.substr(0, equalPos);
    std::string valueStr = line.substr(equalPos + 1);

    // Trim whitespace from key and value
    key.erase(key.begin(), std::find_if(key.begin(), key.end(),
                                        [](unsigned char ch) { return !std::isspace(ch); }));
    key.erase(
        std::find_if(key.rbegin(), key.rend(), [](unsigned char ch) { return !std::isspace(ch); })
            .base(),
        key.end());

    valueStr.erase(valueStr.begin(),
                   std::find_if(valueStr.begin(), valueStr.end(),
                                [](unsigned char ch) { return !std::isspace(ch); }));
    valueStr.erase(std::find_if(valueStr.rbegin(), valueStr.rend(),
                                [](unsigned char ch) { return !std::isspace(ch); })
                       .base(),
                   valueStr.end());

    try
    {
      int value = std::stoi(valueStr);
      config_[key] = value;
    }
    catch (...)
    {
      std::cout << "Warning: Invalid integer value for key '" << key << "': " << valueStr
                << std::endl;
    }
  }

  return true;
}

int ConfigLoader::getInt(const std::string& key, int defaultValue) const
{
  auto it = config_.find(key);
  if (it != config_.end()) return it->second;
  return defaultValue;
}
