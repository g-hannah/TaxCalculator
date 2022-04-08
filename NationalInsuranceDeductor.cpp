#include "NationalInsuranceDeductor.h"

double NationalInsuranceDeductor::Deduct(double amount)
{
  //std::cerr << "NI deduction: received value " << amount << std::endl;

  TaxDatabase* database = TaxDatabase::GetInstance();

  std::vector<double> niBands = database->GetNationalInsuranceBands();
  std::vector<double> niRates = database->GetNationalInsuranceRates();

  const auto TaxForAmount = [](double lower, double higher, double rate) {
    //std::cerr << "lower: " << lower << ", higher: " << higher << ", rate: " << rate << std::endl;
    return rate * (higher - lower);
  };

  double totalTax = 0.0;

  for (std::size_t i = 0, n = niBands.size(); i < n; ++i)
  {
    const double currentBand = niBands[i];
    const double currentRate = niRates[i];

    if (amount <= currentBand)
      break;

    //std::cerr << "band: " << currentBand << std::endl;
    //std::cerr << "rate: " << currentRate << std::endl;

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