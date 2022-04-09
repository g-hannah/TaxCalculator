#include "IncomeTaxDeductor.h"
#include <string>
#include <sstream>

double IncomeTaxDeductor::Deduct(double amount)
{
  TaxDatabase* database = TaxDatabase::GetInstance();

  const std::vector<double> niBands = database->GetIncomeTaxBands();
  const std::vector<double> niRates = database->GetIncomeTaxRates();

  double totalTax = 0.0;
  double amountTaxed = 0.0;

  for (std::size_t i = 0, n = niBands.size(); i < n; ++i)
  {
    const double currentBand = niBands[i];
    const double currentRate = niRates[i];

    if (currentBand > amount)
      break;

    if (i < (n - 1))
    {
      const double nextBand = niBands[i + 1];

      if (amount >= nextBand)
      {
        totalTax += TaxForAmount(currentBand, nextBand, currentRate);
        amountTaxed += (0.0 == currentBand ? 0.0 : (nextBand - currentBand));
      }
      else
      {
        totalTax += TaxForAmount(currentBand, amount, currentRate);
        amountTaxed += (0.0 == currentBand ? 0.0 : (amount - currentBand));
      }
    }
    else
    {
      if (amount > currentBand)
      {
        totalTax += TaxForAmount(currentBand, amount, currentRate);
        amountTaxed += (0.0 == currentBand ? 0.0 : (amount - currentBand));
      }
    }
  }

  std::cerr << "\nIncome Tax\n" << "Total amount taxed: " << amountTaxed << "\nTotal tax: " << totalTax << std::endl;
  return BaseDeductor::Deduct(amount - totalTax);
}