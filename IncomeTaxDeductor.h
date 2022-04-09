#pragma once

#include "BaseDeductor.h"

/**
 * Derived class for deducting income tax.
 * 
 * @author Gary Hannah
 */
namespace UKTax
{
  namespace Deductors
  {
    class IncomeTaxDeductor : public BaseDeductor
    {
    public:
      IncomeTaxDeductor() : region(TaxDatabase::UKRegion::eEngland) { }
      IncomeTaxDeductor(TaxDatabase::UKRegion region) : region(region) { }

      double Deduct(double);

    private:
      TaxDatabase::UKRegion region;
    };
  };
};
