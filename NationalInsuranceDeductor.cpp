#include "NationalInsuranceDeductor.h"

using namespace UKTax::Deductors;

namespace UKTax
{
  namespace Deductors
  {
    double NationalInsuranceDeductor::Deduct(double amount)
    {
      if (0.0 >= amount)
        return amount;

      TaxDatabase* database = TaxDatabase::GetInstance();

      const NationalInsuranceTax& nationalInsuranceTax = database->GetNationalInsuranceTax(this->region);

      TaxValues niBands = nationalInsuranceTax.GetBands();
      TaxValues niRates = nationalInsuranceTax.GetRates();

      TaxValue totalTax = 0.0;
      TaxValue amountTaxed = 0.0;

      for (std::size_t i = 0, n = niBands.size(); i < n; ++i)
      {
        const TaxValue currentBand = niBands[i];
        const TaxValue currentRate = niRates[i];

        if (currentBand > amount)
          break;

        if (i < (n - 1))
        {
          const TaxValue nextBand = niBands[i + 1];

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
      return BaseTaxDeductor::Deduct(amount - totalTax);
    }
  };
};