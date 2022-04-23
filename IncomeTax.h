#pragma once

#include "Tax.h"

/**
 * Class for storing specific income tax bands and rates
 * 
 * @author Gary Hannah
 */
namespace UKTax
{
  class IncomeTax : public Tax
  {
  private:
    TaxValue noAllowanceThreshold;

  public:
    IncomeTax(TaxValues bands, TaxValues rates, TaxValue noAllowanceThreshold) :
      noAllowanceThreshold(noAllowanceThreshold)
    {
      Tax::bands = bands;
      Tax::rates = rates;
    }

    TaxValue GetNoAllowanceThreshold() const;
  };
};
