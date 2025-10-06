#include "core/Runner.h"

#include <algorithm>
#include <fstream>
#include <memory>
#include <vector>

#include "config/Config.h"
#include "config/ConfigLoader.h"
#include "config/Constants.h"
#include "core/Graph.h"
#include "io/FileManager.h"
#include "logger/Logger.h"
#include "probability/Dynp.h"
#include "probability/OldDynp.h"
#include "structure/K22Store.h"
#include "util/ExistingGraphs.h"

void Runner::printResults(const std::string &filename, const Results &results)
{
  std::ofstream outfile;
  outfile.open(filename);
  if (!outfile.is_open())
  {
    throw std::runtime_error("Could not open log file: " + filename);
  }
  size_t siz = results.size();
  for (size_t i = 0; i < siz; ++i)
  {
    for (size_t j = 0; j < siz; ++j)
    {
      outfile << results[i][j] << " ";
    }
    outfile << std::endl;
  }
}

void Runner::updateGraphsToSave(Graph (&graphsToSave)[Constants::MAX_GRAPHS_TO_SAVE], Graph &adj,
                                int maxGraphsToSave)
{
  int edgeNum = adj.edges;
  if (edgeNum > graphsToSave[maxGraphsToSave - 1].edges &&
      std::all_of(std::begin(graphsToSave), std::end(graphsToSave),
                  [edgeNum](const auto &a) { return a.edges != edgeNum; }))
  {
    graphsToSave[maxGraphsToSave - 1] = adj;
    std::sort(std::begin(graphsToSave), std::end(graphsToSave),
              [](const auto &a, const auto &b) { return a.edges > b.edges; });
  }
}

int Runner::addTrivialEdges(Graph &adj, Logs &logs)  // TODO! becnchmark the other version where
                                                     // it picks all the edges in random order
{
  int m = adj.m;
  int n = adj.n;
  int addedEdges = 0;
  for (int u = 0; u < m; ++u)
  {
    for (int v = 0; v < n; ++v)
    {
      if (!adj[u][v] && !kstStore_->createsKst(adj, u, v))
      {
        adj.addEdge(u, v);
        ++addedEdges;
      }
    }
  }
  logs.add(adj.edges);
  return addedEdges;
}

std::unique_ptr<Probabilities> Runner::makeProb(int type, int m, int n, int s, int t)
{
  switch (type)
  {
    case 0:
      return std::make_unique<Dynp>(m, n, s, t);
    case 1:
      return std::make_unique<OldDynp>(m, n, s, t);
    default:
      throw std::invalid_argument("Unsupported Probabilities type");
  }
}
std::unique_ptr<KstStore> Runner::makeKstStore(int s, int t)
{
  if (s == 2 && t == 2)
  {
    return std::make_unique<K22Store>();
  }
  // else if (s == 3 && t == 3)
  // {
  //   return std::make_unique<K33Store>();
  // }
  else
  {
    throw std::invalid_argument("Unsupported K_{s,t} store type");
  }
}

void Runner::run()
{
  std::vector<std::vector<int>> results(39, std::vector<int>(39, 0));
  getConfigInstance().loadConfig(std::string(Constants::CONFIG_FILE));
  s_ = getConfigInstance().s;
  t_ = getConfigInstance().t;
  prob_ = makeProb(getConfigInstance().probabilityType, getConfigInstance().min,
                   getConfigInstance().min, s_, t_);
  kstStore_ = makeKstStore(s_, t_);
  FileManager::init();
  runFullIteration(getConfigInstance().min, getConfigInstance().max, getConfigInstance().runCount,
                   getConfigInstance().iterations, getConfigInstance().insideIterations);
}

void Runner::runFullIteration(int min, int max, int runCount, int iterations, int insideIterations)
{
  for (int runId = 0; runId < runCount; ++runId)
  {
    FileManager::loadExistingGraphs();
    Results res = runInRange(min, max, iterations, insideIterations, runId);
    printResults(getConfigInstance().resultsFileName(runId), res);
    for (size_t i = 0; i < res.size(); ++i)
    {
      for (size_t j = 0; j < res[i].size(); ++j)
      {
        if (res[i][j] > results_[i][j])
        {
          results_[i][j] = res[i][j];
        }
      }
    }
  }
  printResults(getConfigInstance().finalResultsFileName(), results_);
}

Results Runner::runInRange(int min, int max, int iterations, int insideIterations, int runId)
{
  int maxGraphsToSave = getConfigInstance().maxGraphsToSave;
  Results res = {};
  Logger logger(getConfigInstance().logFileName(runId));
  for (int m = min; m <= max; ++m)  // TODO szimmetrikus esetek
  {
    for (int n = min; n <= max; ++n)
    {
      Graph graphsToSave[Constants::MAX_GRAPHS_TO_SAVE] =
          {};  // because its 0 initialized all the edges will be 0
      Graph adj(m, n);
      ExistingGraphs existingGraphs(m, n, runId % 2 + 1,
                                    kstStore_.get());  // TODO we need to get this from Config
      Logs logs(m, n, s_, t_);
      prob_->reInitialize(m, n, s_, t_);
      for (int iter = 0; iter < iterations; iter++)
      {
        prob_->clear();
        kstStore_->clear();
        adj = existingGraphs.getStartingGraph(iter);
        adj.m = m;
        adj.n = n;
        runIteration(adj, insideIterations, m, n);
        addTrivialEdges(adj, logs);
        updateGraphsToSave(graphsToSave, adj, maxGraphsToSave);
      }
      res[m - 2][n - 2] = graphsToSave[0].edges;
      logger.log(logs);
      for (int i = 0; i < maxGraphsToSave; ++i)
      {
        if (graphsToSave[i].edges == 0) break;
        FileManager::print_graph(graphsToSave[i],
                                 getConfigInstance().outputFilename(m, n, graphsToSave[i].edges));
      }
    }
  }
  return res;
}

void Runner::runIteration(Graph &adj, int insideIterations, int m, int n)
{
  // TODO comment out the code below and watch for performance changes
  // also comment out the addedEdges part from runner.cpp and from K22Store
  Graph best(adj);  // code to comment out // code to comment out
  for (int iter = 0; iter < insideIterations; ++iter)
  {
    if (adj.edges > best.edges)  // code to comment out
    {
      best = adj;
    }
    for (int u = 0; u < m; ++u)
    {
      for (int v = 0; v < n; ++v)
      {
        if (adj[u][v] == 0)
        {
          double p = prob_->get_p(u, v);
          double rand_val = Util::randDouble();
          if (rand_val < p)
          {
            prob_->add_edge(u, v);
            kstStore_->storeKst(adj, u, v);
            adj.addEdge(u, v);
          }
        }
      }
    }
    while (!kstStore_->empty())
    {
      kstStore_->reflipCircle(adj, *prob_);
      kstStore_->reevalCircles(adj);
    }
  }
  if (best.edges > adj.edges)  // TODO code to comment out probably
  {
    adj = best;
  }
}