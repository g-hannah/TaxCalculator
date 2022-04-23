#include "IncomeTaxDeductor.h"
#include <string>
#include <sstream>

using namespace UKTax::Deductors;

double IncomeTaxDeductor::Deduct(double amount)
{
  if (0.0 >= amount)
    return amount;

  TaxDatabase* database = TaxDatabase::GetInstance();

  const IncomeTax& incomeTax = database->GetIncomeTax(region);

  const TaxValues niBands = incomeTax.GetBands();

  /*
   * Not const as we may need to change
   * the value of the first rate (see below)
   */
  TaxValues niRates = incomeTax.GetRates();
  const double noAllowanceThreshold = incomeTax.GetNoAllowanceThreshold();

  /*
   * If the gross salary is greater than this threshold,
   * there is no "personal allowance" anymore. Thus,
   * the first rate (0.0) becomes the same as the second.
   */
  if (gross > noAllowanceThreshold)
  {
    niRates[0] = niRates[1];
  }

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
  return BaseTaxDeductor::Deduct(amount - totalTax);
}