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
#include "probability/OldDynpSymmetric.h"
#include "structure/KstStore.h"
#include "util/ExistingGraphs.h"

void Runner::printResults(const std::string& filename, const Results& results)
{
  std::ofstream outfile;
  outfile.open(filename);
  if (!outfile.is_open())
  {
    throw std::runtime_error("Could not open results file: " + filename);
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

void Runner::updateGraphsToSave(Graph (&graphsToSave)[Constants::MAX_GRAPHS_TO_SAVE], Graph& adj,
                                int maxGraphsToSave)
{
  int edgeNum = adj.edges;
  if (edgeNum > graphsToSave[maxGraphsToSave - 1].edges &&
      std::all_of(std::begin(graphsToSave), std::end(graphsToSave),
                  [edgeNum](const auto& a) { return a.edges != edgeNum; }))
  {
    graphsToSave[maxGraphsToSave - 1] = adj;
    std::sort(std::begin(graphsToSave), std::end(graphsToSave),
              [](const auto& a, const auto& b) { return a.edges > b.edges; });
  }
}

int Runner::addTrivialEdges(Graph& adj, Logs& logs)  // TODO! becnchmark the other version where
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
    case 2:
      return std::make_unique<OldDynpSymmetric>(m, n, s, t);  // GEOMETRIC_MEAN
    case 20:
      return std::make_unique<OldDynpSymmetric>(
          m, n, s, t, OldDynpSymmetricType::HARMONIC_MEAN);  // HARMONIC_MEAN
    case 21:
      return std::make_unique<OldDynpSymmetric>(
          m, n, s, t, OldDynpSymmetricType::GEOMETRIC_MEAN);  // GEOMETRIC_MEAN
    case 22:
      return std::make_unique<OldDynpSymmetric>(
          m, n, s, t, OldDynpSymmetricType::ARITHMETIC_MEAN);  // ARITHMETIC_MEAN
    case 23:
      return std::make_unique<OldDynpSymmetric>(
          m, n, s, t, OldDynpSymmetricType::QUADRATIC_MEAN);  // QUADRATIC_MEAN
    default:
      throw std::invalid_argument("Unsupported Probabilities type");
  }
}

void Runner::run(const char* runGroup)
{
  if (runGroup == nullptr || std::string(runGroup).empty())
  {
    getConfigInstance().loadConfig(std::string(Constants::CONFIG_FILE));
  }
  else
  {
    getConfigInstance().outputPrefix = "experiments/" + std::string(runGroup);
    getConfigInstance().loadConfig("experiments/" + std::string(runGroup) + "_" +
                                   std::string(Constants::CONFIG_FILE));
  }
  s_ = getConfigInstance().s;
  t_ = getConfigInstance().t;
  prob_ = makeProb(getConfigInstance().probabilityType, getConfigInstance().min,
                   getConfigInstance().min, s_, t_);
  kstStore_ = Util::createKstStore(s_, t_);
  FileManager::init(getConfigInstance().graphsDirectory());
  runFullIteration(getConfigInstance().min, getConfigInstance().max, getConfigInstance().runCount,
                   getConfigInstance().iterations, getConfigInstance().insideIterations);
}

void Runner::runFullIteration(int min, int max, int runCount, int iterations, int insideIterations)
{
  for (int runId = 0; runId < runCount; ++runId)
  {
    FileManager::loadExistingGraphs(getConfigInstance().graphsDirectory());
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
  int insideIterationArray[] = {1, 3, 5, 7, 9};
  if (!getConfigInstance().withIterations)
  {
    std::fill(std::begin(insideIterationArray), std::end(insideIterationArray), 0);
  }
  int maxGraphsToSave = getConfigInstance().maxGraphsToSave;
  Results res = {};
  Logger logger(getConfigInstance().logFileName(runId));
  for (int m = min; m <= max; ++m)  // TODO szimmetrikus esetek
  {
    for (int n = (s_ == t_) ? m : min; n <= max; ++n)
    {
      Graph graphsToSave[Constants::MAX_GRAPHS_TO_SAVE] =
          {};  // because its 0 initialized all the edges will be 0
      Graph adj(m, n);
      int operationType = runId % 2 + 1;  // alternate between adding to m and n (1 and 2
      if (m == n) operationType = 1;      // if m==n just get the existing graphs
      ExistingGraphs existingGraphs(m, n, operationType,
                                    kstStore_.get());  // TODO we need to get this from Config
      Logs logs(m, n, s_, t_);
      for (int iter = 0; iter < iterations; iter++)
      {
        kstStore_->clear();
        adj = std::move(existingGraphs.getStartingGraph(iter));
        prob_->reInitialize(adj, s_, t_);
        adj.m = m;
        adj.n = n;
        runIteration(adj, insideIterationArray[iter % 5], m, n);
        addTrivialEdges(adj, logs);
        updateGraphsToSave(graphsToSave, adj, maxGraphsToSave);
      }
      res[m - 2][n - 2] = graphsToSave[0].edges;
      logger.log(logs);
      for (int i = 0; i < maxGraphsToSave; ++i)
      {
        if (graphsToSave[i].edges == 0) break;
        FileManager::addGraphToExisting(graphsToSave[i]);
        FileManager::print_graph(graphsToSave[i],
                                 getConfigInstance().outputFilename(m, n, graphsToSave[i].edges));
      }
    }
  }
  return res;
}

void Runner::runIteration(Graph& adj, int insideIterations, int m, int n)
{
  // TODO comment out the code below and watch for performance changes
  // also comment out the addedEdges part from runner.cpp and from K22Store
  // Graph best(adj);  // code to comment out // code to comment out
  for (int iter = 0; iter < insideIterations; ++iter)
  {
    /*if (adj.edges > best.edges)  // code to comment out
    {
      best = adj;
    }*/
    for (int u = 0; u < m; ++u)
    {
      for (int v = 0; v < n; ++v)
      {
        if (adj[u][v] == 0)
        {
          double p = prob_->get_p(u, v) *
                     getConfigInstance().probabilityMultiplier;  // TODO: switch it to config if it
                                                                 // can read double values
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
  /*if (best.edges > adj.edges)  // TODO code to comment out probably
  {
    adj = best;
  }*/
}