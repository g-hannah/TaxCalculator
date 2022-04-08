#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include "BaseDeductor.h"
#include "TaxDatabase.h"

class IncomeTaxDeductor : public BaseDeductor
{
public:
  IncomeTaxDeductor() { }
  double Deduct(double);
};
