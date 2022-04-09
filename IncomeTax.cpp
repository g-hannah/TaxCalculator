#include "IncomeTax.h"

using namespace UKTax;

std::vector<double> IncomeTax::GetBands()
{
  return bands;
}

std::vector<double> IncomeTax::GetRates()
{
  return rates;
}

double IncomeTax::GetNoAllowanceThreshold()
{
  return noAllowanceThreshold;
}