#pragma once

#include "NationalInsuranceDeductor.h"
#include "IncomeTaxDeductor.h"
#include "StudentLoanDeductor.h"

/**
 * Builder class for building tax deductors.
 * 
 * @author Gary Hannah
 */
namespace UKTax
{
  namespace Deductors
  {
    class DeductorBuilder
    {
    private:
      TaxDatabase::UKRegion region;
      TaxDatabase::StudentLoanPlan plan;
      bool hasStudentLoan; // do we need a student loan deductor?
      double salary;

      /*
       * Both must be true when we call Build()
       */
      bool gotSalary;
      bool gotRegion;

    public:
      DeductorBuilder() :
        salary(0.0),
        region(TaxDatabase::UKRegion::eAllRegions),
        hasStudentLoan(false),
        gotSalary(false),
        gotRegion(false) { }

      DeductorBuilder* SetSalary(double);
      DeductorBuilder* SetRegion(TaxDatabase::UKRegion);
      DeductorBuilder* SetStudentLoanPlan(TaxDatabase::StudentLoanPlan);
      std::unique_ptr<BaseDeductor> Build();
    };
  };
};

