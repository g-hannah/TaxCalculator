#include "IncomeTaxDeductor.h"
#include <string>
#include <sstream>

double IncomeTaxDeductor::Deduct(double amount)
{
  TaxDatabase* database = TaxDatabase::GetInstance();

  const std::vector<double> niBands = database->GetIncomeTaxBands();
  const std::vector<double> niRates = database->GetIncomeTaxRates();

  double totalTax = 0.0;

  const auto TaxForAmount = [](double lower, double higher, double rate) {

    return rate * (higher - lower);
  };

  for (std::size_t i = 0, n = niBands.size(); i < n; ++i)
  {
    const double currentBand = niBands[i];
    const double currentRate = niRates[i];

    if (amount <= currentBand)
      break;

    if (i < (n - 1))
    {
      const double nextBand = niBands[i + 1];

      if (amount >= nextBand)
      {
        totalTax += TaxForAmount(currentBand, nextBand, currentRate);
      }
      else
      {
        totalTax += TaxForAmount(currentBand, amount, currentRate);
      }
    }
    else
    {
      if (amount > currentBand)
      {
        totalTax += TaxForAmount(currentBand, amount, currentRate);
      }
    }
  }

  return BaseDeductor::Deduct(amount - totalTax);
}