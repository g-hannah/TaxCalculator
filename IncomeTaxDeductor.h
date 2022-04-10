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
      IncomeTaxDeductor() :
        region(TaxDatabase::UKRegion::eEngland),
        salary(0.0)
      { }

      IncomeTaxDeductor(TaxDatabase::UKRegion region, double salary) :
        region(region),
        salary(salary)
      { }

      double Deduct(double);

    private:
      TaxDatabase::UKRegion region;
      double salary;
    };
  };
};
