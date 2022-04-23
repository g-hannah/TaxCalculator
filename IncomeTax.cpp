#include "IncomeTax.h"

namespace UKTax
{
  TaxValue IncomeTax::GetNoAllowanceThreshold() const
  {
    return noAllowanceThreshold;
  }
};