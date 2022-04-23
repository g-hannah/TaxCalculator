#pragma once

#include "Tax.h"

namespace UKTax
{
  class StudentLoanTax : public Tax
  {
  public:
    StudentLoanTax()
    {}

    StudentLoanTax(TaxValue band, TaxValue rate)
    {
      Tax::band = band;
      Tax::rate = rate;
    }
  };
};
