#pragma once

#include "BaseDeductor.h"

/**
 * Derived class for deducting student loan tax.
 * 
 * @author Gary Hannah
 */
namespace UKTax
{
  namespace Deductors
  {
    class StudentLoanDeductor : public BaseDeductor
    {
    private:
      TaxDatabase::StudentLoanPlan plan;
      double salary;
    public:

      StudentLoanDeductor(TaxDatabase::StudentLoanPlan plan, double salary)
        : plan(plan), salary(salary) { }

      double Deduct(double);
    };
  };
};