#pragma once

#include "BaseTaxDeductor.h"

/**
 * Derived class for deducting national insurance.
 * 
 * @author Gary Hannah
 */
namespace UKTax
{
  namespace Deductors
  {
    class NationalInsuranceDeductor : public BaseTaxDeductor
    {
    public:
      NationalInsuranceDeductor(double gross, TaxDatabase::UKRegion region)
      {
        BaseTaxDeductor::gross = gross;
        BaseTaxDeductor::region = region;
      }

      virtual double Deduct(double);
    };
  };
};

