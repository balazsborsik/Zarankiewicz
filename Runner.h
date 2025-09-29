#ifndef RUNNER_H
#define RUNNER_H

#include <memory>

#include "Graph.h"
#include "KstStore.h"
#include "Logs.h"
#include "Probabilities.h"

using Results = std::array<std::array<int, Constants::MAX_SIZE - 1>, Constants::MAX_SIZE - 1>;

class Runner
{
 private:
  int s_, t_;
  std::unique_ptr<Probabilities> prob_;
  std::unique_ptr<KstStore> kstStore_;
  Results results_ = {};

  void runFullIteration(int min, int max, int runCount, int iterations, int insideIterations);

  Results runInRange(int min, int max, int iterations, int insideIterations, int runId);

  void runIteration(Graph &adj, int insideIterations, int m, int n);

  std::unique_ptr<Probabilities> makeProb(int type, int m, int n, int s, int t);
  std::unique_ptr<KstStore> makeKstStore(int s, int t);
  int addTrivialEdges(Graph &adj, Logs &logs);
  void printResults(const std::string &filename, const Results &results);
  void updateGraphsToSave(std::pair<int, Graph> (&graphs)[Constants::MAX_GRAPHS_TO_SAVE],
                          Graph &adj, int edgeNum, int maxGraphsToSave);
  void getStartingGraph() {}

 public:
  Runner() {};
  void run();
};

#endif  // RUNNER_H
