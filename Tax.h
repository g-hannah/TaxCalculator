#pragma once

#include <vector>

namespace UKTax
{
  class Tax
  {
  public:
    virtual std::vector<double> GetBands() = 0;
    virtual std::vector<double> GetRates() = 0;
  };
}