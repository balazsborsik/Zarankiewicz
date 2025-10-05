#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#include <string>
#include <unordered_map>

class ConfigLoader
{
 public:
  bool loadConfig(const std::string& filename);
  int getInt(const std::string& key, int defaultValue) const;

 private:
  std::unordered_map<std::string, int> config_;
};

#endif  // CONFIGLOADER_H