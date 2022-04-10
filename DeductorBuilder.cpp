#include "DeductorBuilder.h"

namespace UKTax
{
  namespace Deductors
  {
    DeductorBuilder* DeductorBuilder::SetRegion(TaxDatabase::UKRegion region)
    {
      this->region = region;
      gotRegion = true;
      return this;
    }

    DeductorBuilder* DeductorBuilder::SetStudentLoanPlan(TaxDatabase::StudentLoanPlan plan)
    {
      hasStudentLoan = true;
      this->plan = plan;
      return this;
    }

    DeductorBuilder* DeductorBuilder::SetSalary(double salary)
    {
      if (0.0 >= salary)
        throw std::exception();

      this->salary = salary;
      gotSalary = true;
      return this;
    }

    std::unique_ptr<BaseDeductor> DeductorBuilder::Build()
    {
      if (!gotSalary || 0.0 >= salary || !gotRegion)
        throw std::exception();

      std::unique_ptr<BaseDeductor> deductor = std::make_unique<NationalInsuranceDeductor>();
      BaseDeductor* ptr = nullptr;

      ptr = deductor->SetNext(new IncomeTaxDeductor(region, salary));

      if (hasStudentLoan)
        ptr->SetNext(new StudentLoanDeductor(plan, salary));

      return std::move(deductor);
    }
  };
};