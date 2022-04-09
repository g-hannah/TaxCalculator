#include "NationalInsuranceDeductor.h"

using namespace UKTax::Deductors;

double NationalInsuranceDeductor::Deduct(double amount)
{
  if (0.0 >= amount)
    return amount;

  TaxDatabase* database = TaxDatabase::GetInstance();

  std::vector<double> niBands = database->GetNationalInsuranceBands();
  std::vector<double> niRates = database->GetNationalInsuranceRates();

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

  std::cerr << "\nNational Insurance\n" << "Total amount taxed: " << amountTaxed << "\nTotal tax: " << totalTax << std::endl;
  return BaseDeductor::Deduct(amount - totalTax);
}