#pragma once

#include "BaseTaxDeductor.h"

/**
 * Derived class for deducting income tax.
 * 
 * @author Gary Hannah
 */
namespace UKTax
{
  namespace Deductors
  {
    class IncomeTaxDeductor : public BaseTaxDeductor
    {
    public:
      IncomeTaxDeductor()
      {}

      IncomeTaxDeductor(double gross, TaxDatabase::UKRegion region)
      {
        this->gross = gross;
        this->region = region;
      }

      virtual double Deduct(double);
    };
  };
};
