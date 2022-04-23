#pragma once

#include <vector>

/**
 * Generic interface for tax classes
 * 
 * @author Gary Hannah
 */
namespace UKTax
{
  using TaxValue = double;
  using TaxValues = std::vector<TaxValue>;
  using TaxValuesIterator = std::vector<TaxValue>::iterator;

  class Tax
  {
  protected:
    TaxValue band;
    TaxValue rate;
    TaxValues bands;
    TaxValues rates;
  public:
    virtual double GetBand() const;
    virtual double GetRate() const;
    virtual TaxValues GetBands() const;
    virtual TaxValues GetRates() const;
  };
}