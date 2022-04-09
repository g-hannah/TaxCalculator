#include <iostream>
#include "IncomeTaxDeductor.h"
#include "NationalInsuranceDeductor.h"
#include "StudentLoanDeductor.h"

int main()
{
  const double salary = 30000.0;
  BaseDeductor* deductor = new NationalInsuranceDeductor();

  deductor
    ->SetNext(new IncomeTaxDeductor())
    ->SetNext(new StudentLoanDeductor(TaxDatabase::StudentLoanPlan::ePlan4, salary));

  const double net = deductor->Deduct(salary);

  std::cerr << "\nNet Salary\n" << net << std::endl;

  return 0;
}