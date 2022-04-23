#include "Tax.h"

namespace UKTax
{
  TaxValue Tax::GetBand() const
  {
    return this->band;
  }

  TaxValue Tax::GetRate() const
  {
    return this->rate;
  }

  TaxValues Tax::GetBands() const
  {
    return this->bands;
  }

  TaxValues Tax::GetRates() const
  {
    return this->rates;
  }
};