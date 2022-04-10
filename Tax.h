#pragma once

#include <vector>

/**
 * Generic interface for tax classes
 * 
 * @author Gary Hannah
 */
namespace UKTax
{
  class Tax
  {
  public:
    virtual std::vector<double> GetBands() = 0;
    virtual std::vector<double> GetRates() = 0;
  };
}