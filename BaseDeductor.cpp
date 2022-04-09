#include "BaseDeductor.h"

double BaseDeductor::TaxForAmount(double a, double b, double rate)
{
  return rate * (abs(b - a));
}