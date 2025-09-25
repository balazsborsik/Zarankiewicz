#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <fstream>
#include <iostream>
#include <map>

#include "Graph.h"
#include "Logs.h"

class FileManager
{
 public:
  static Graph create_from_file(int m, int n, const std::string& filename);
  static void print_graph(const Graph& graph, const std::string& = "");
  static void init();
  static void log(const Logs& logs);
  static void closeLogFile();
  static Graph loadTextWholeFile(const std::string& filename, int m,
                                 int n);  // TODO try this to measure performance improvement
  static void loadExistingFilenames();  // WARNING: this will cache all existing filenames in memory
                                        // if you print additional graphs you need to call this
                                        // again to refresh the cache
 private:
  static std::map<std::pair<int, int>,
                  std::array<std::pair<int, std::string>, Constants::MAX_GRAPHS_TO_SAVE>>
      existingFilenames_;
  static std::ofstream logFile_;
  static void print_graph(const Graph& graph, std::ostream& out);
};

#endif  // FILEMANAGER_H
