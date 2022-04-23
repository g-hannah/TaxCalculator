#include "DeductorBuilder.h"

namespace UKTax
{
  namespace Deductors
  {
    DeductorBuilder* DeductorBuilder::SetRegion(TaxDatabase::UKRegion region)
    {
      this->region = region;
      this->gotRegion = true;

      return this;
    }

    DeductorBuilder* DeductorBuilder::SetStudentLoanPlan(TaxDatabase::StudentLoanPlan plan)
    {
      this->hasStudentLoan = true;
      this->plan = plan;

      return this;
    }

    DeductorBuilder* DeductorBuilder::SetGross(double gross)
    {
      if (0.0 >= gross)
        throw std::exception();

      this->gross = gross;
      this->gotGross = true;

      return this;
    }

    std::unique_ptr<BaseTaxDeductor> DeductorBuilder::Build()
    {
      if (!gotGross || 0.0 >= gross || !gotRegion)
        throw std::exception("Attempting to build a deductor without required set parameters.");

      std::unique_ptr<BaseTaxDeductor> deductor = std::make_unique<NationalInsuranceDeductor>(gross, region);
      BaseTaxDeductor* ptr = nullptr;

      ptr = deductor->SetNext(new IncomeTaxDeductor(gross, region));

      if (hasStudentLoan)
        ptr->SetNext(new StudentLoanDeductor(gross, plan));

      return std::move(deductor);
    }
  };
};