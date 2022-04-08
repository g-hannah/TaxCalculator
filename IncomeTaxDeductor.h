#pragma once

#include "BaseDeductor.h"

/**
 * Derived class for deducting income tax.
 * 
 * @author Gary Hannah
 */
class IncomeTaxDeductor : public BaseDeductor
{
public:
  IncomeTaxDeductor() { }
  double Deduct(double);
};
