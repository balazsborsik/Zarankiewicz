#ifndef OLD_DYNP_SYMMETRIC_H
#define OLD_DYNP_SYMMETRIC_H

#include "probability/Probabilities.h"

enum class OldDynpSymmetricType
{
  HARMONIC_MEAN,
  GEOMETRIC_MEAN,
  ARITHMETIC_MEAN,
  QUADRATIC_MEAN
};

class OldDynpSymmetric : public Probabilities
{
 private:
  OldDynpSymmetricType meanType = OldDynpSymmetricType::GEOMETRIC_MEAN;

  double get_multiplier_m(int v_m) const;
  double get_multiplier_n(int v_n) const;

  double compute_m_term(int v_m, int v_n) const;
  double compute_n_term(int v_m, int v_n) const;

  double apply_mean(double a, double b) const;

 public:
  OldDynpSymmetric(int m, int n, int s, int t,
                   OldDynpSymmetricType type = OldDynpSymmetricType::GEOMETRIC_MEAN)
      : Probabilities(m, n, s, t), meanType(type)
  {
  }
  OldDynpSymmetric(const Graph& graph, int s, int t,
                   OldDynpSymmetricType type = OldDynpSymmetricType::GEOMETRIC_MEAN)
      : Probabilities(graph, s, t), meanType(type)
  {
  }
  double get_p(int v_m, int v_n) override;
};

#endif  // OLD_DYNP_SYMMETRIC_H
