#pragma once

#include "BaseDeductor.h"

/**
 * Derived class for deducting national insurance.
 * 
 * @author Gary Hannah
 */
class NationalInsuranceDeductor : public BaseDeductor
{
public:
  NationalInsuranceDeductor() { }
  double Deduct(double);
};

