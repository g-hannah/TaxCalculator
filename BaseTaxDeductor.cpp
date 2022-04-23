#include "BaseTaxDeductor.h"

namespace UKTax
{
  namespace Deductors
  {
    double BaseTaxDeductor::TaxForAmount(double lower, double higher, double rate)
    {
      return rate * (abs(higher - lower));
    }
  };
};