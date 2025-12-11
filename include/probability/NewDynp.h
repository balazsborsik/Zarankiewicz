#pragma once

#include "probability/Probabilities.h"

class NewDynp : public Probabilities
{
 public:
  using Probabilities::Probabilities;

  double get_p(int v_m, int v_n) override;
};
