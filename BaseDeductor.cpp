#include "BaseDeductor.h"

using namespace UKTax::Deductors;

double BaseDeductor::TaxForAmount(double a, double b, double rate)
{
  return rate * (abs(b - a));
}