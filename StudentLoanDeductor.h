#pragma once

#include "BaseTaxDeductor.h"

/**
 * Derived class for deducting student loan tax.
 * 
 * @author Gary Hannah
 */
namespace UKTax
{
  namespace Deductors
  {
    class StudentLoanDeductor : public BaseTaxDeductor
    {
    private:
      TaxDatabase::StudentLoanPlan plan;
    public:

      StudentLoanDeductor(double gross, TaxDatabase::StudentLoanPlan plan) :
        plan(plan)
      {
        this->gross = gross;
      }

      virtual double Deduct(double);
    };
  };
};