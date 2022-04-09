#pragma once

#include "Tax.h"

namespace UKTax
{
  class IncomeTax : public Tax
  {
  private:
    std::vector<double> bands;
    std::vector<double> rates;
    double noAllowanceThreshold;

  public:
    IncomeTax(std::vector<double> bands, std::vector<double> rates, double noAllowanceThreshold) :
      bands(bands),
      rates(rates),
      noAllowanceThreshold(noAllowanceThreshold)
    {
    }

    std::vector<double> GetBands() override;
    std::vector<double> GetRates() override;
    double GetNoAllowanceThreshold();
  };
};
