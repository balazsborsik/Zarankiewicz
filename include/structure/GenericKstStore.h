#ifndef GENERIC_KST_STORE_H
#define GENERIC_KST_STORE_H

#include "config/Constants.h"
#include "core/Graph.h"
#include "structure/Kst.h"
#include "structure/KstStore.h"

class Probabilities;

template <typename T, int N>
struct Fixvector
{
  T arr[N];
  int size_;

  Fixvector() : size_(0) {}
  void push_back(T val) { arr[size_++] = val; }
  void pop_back() { --size_; }
  int size() const { return size_; }
  T* begin() { return arr; }
  const T* begin() const { return arr; }

  T* end() { return arr + size_; }
  const T* end() const { return arr + size_; }

  T& operator[](int index) { return arr[index]; }
  const T& operator[](int index) const { return arr[index]; }
};

class GenericKstStore : public KstStore
{
 private:
  int s_;
  int t_;
  std::vector<Kst<Constants::MAX_S, Constants::MAX_T>> circles_;
  Graph edges_in_circles_;

 public:
  GenericKstStore(int s, int t);
  ~GenericKstStore() override = default;

  int getS() const override { return s_; }
  int getT() const override { return t_; }
  bool empty() override { return circles_.empty(); }
  void clear() override;

  bool createsKst(const Graph& adj, int u, int v) const override;
  void storeKst(const Graph& adj, int u, int v) override;
  void reevalCircles(const Graph& adj) override;
  bool reflipCircle(Graph& adj, Probabilities& prob) override;

 private:
  bool checkRecurseU(const Graph& adj, const Fixvector<int, Constants::MAX_SIZE>& candidates,
                     int start_idx, Fixvector<int, Constants::MAX_S>& current_u, int fixed_v) const;

  void storeRecurseU(const Graph& adj, const Fixvector<int, Constants::MAX_SIZE>& candidates,
                     int start_idx, Fixvector<int, Constants::MAX_S>& current_u, int fixed_v);

  void storeRecurseV(const Fixvector<int, 40>& candidates, int start_idx,
                     const Fixvector<int, Constants::MAX_S>& final_u,
                     Fixvector<int, Constants::MAX_T>& current_v);

  void decrementCounts(const Kst<Constants::MAX_S, Constants::MAX_T>& circle);
};

#endif  // GENERIC_KST_STORE_H