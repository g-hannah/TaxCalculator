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
      /*
       * Some taxes differ by region (income tax).
       */
      TaxDatabase::UKRegion region;

      /*
       * If student loan deductor is required,
       * it needs to know the student loan plan.
       */
      TaxDatabase::StudentLoanPlan plan;

      /*
       * Do we need a student loan deductor?
       */
      bool hasStudentLoan;

      /*
       * Some types of tax determine if tax is necessary based on
       * the gross amount (and not the net amount after other taxes
       * have been deducted), and some calculate the amount to deduct
       * on the gross and not the current net.
       */
      double gross;

      /*
       * Both must be true when we call Build()
       */
      bool gotGross;
      bool gotRegion;

    public:
      DeductorBuilder() :
        gross(0.0),
        region(TaxDatabase::UKRegion::eAllRegions),
        hasStudentLoan(false),
        gotGross(false),
        gotRegion(false)
      {}

      DeductorBuilder* SetGross(double);
      DeductorBuilder* SetRegion(TaxDatabase::UKRegion);
      DeductorBuilder* SetStudentLoanPlan(TaxDatabase::StudentLoanPlan);
      std::unique_ptr<BaseTaxDeductor> Build();
    };
  };
};

