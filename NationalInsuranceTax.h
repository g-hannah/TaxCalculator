#pragma once

#include "Tax.h"

namespace UKTax
{
  class NationalInsuranceTax : public Tax
  {
  public:
    NationalInsuranceTax()
    {}

    NationalInsuranceTax(TaxValues bands, TaxValues rates)
    {
      Tax::bands = bands;
      Tax::rates = rates;
    }
  };
};
