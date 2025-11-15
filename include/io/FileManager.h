#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <array>
#include <fstream>
#include <iostream>
#include <map>

#include "core/Graph.h"

class FileManager
{
 public:
  static Graph create_from_file(int m, int n, const std::string& filename);
  static void print_graph(const Graph& graph, const std::string& filename = "");
  static void init(const std::string& directory);
  static void addGraphToExisting(const Graph& graph);
  static Graph loadTextWholeFile(
      const std::string& filename, int m,
      int n);  // TODO try this to measure performance improvement against create_from_file()
  static void loadExistingGraphs(
      const std::string& directory);  // WARNING: this will cache all existing filenames in memory
                                      // if you print additional graphs you need to call this
                                      // again to refresh the cache
  static std::array<Graph, Constants::MAX_GRAPHS_TO_SAVE> getExistingGraphs(int m, int n);

 private:
  static std::array<Graph, Constants::MAX_GRAPHS_TO_SAVE> existingGraphs_[Constants::MAX_SIZE + 1]
                                                                         [Constants::MAX_SIZE + 1];
  static void print_graph(const Graph& graph, std::ostream& out);
};

#endif  // FILEMANAGER_H
