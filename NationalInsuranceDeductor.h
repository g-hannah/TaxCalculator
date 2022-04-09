#pragma once

#include "BaseDeductor.h"

/**
 * Derived class for deducting national insurance.
 * 
 * @author Gary Hannah
 */
namespace UKTax
{
  namespace Deductors
  {
    class NationalInsuranceDeductor : public BaseDeductor
    {
    public:
      NationalInsuranceDeductor() { }
      double Deduct(double);
    };
  };
};

