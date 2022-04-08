#pragma once

#include "BaseDeductor.h"

class NationalInsuranceDeductor : public BaseDeductor
{
public:
  NationalInsuranceDeductor() { }
  double Deduct(double);
};

