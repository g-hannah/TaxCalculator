#include <iostream>
#include "IncomeTaxDeductor.h"
#include "NationalInsuranceDeductor.h"

int main()
{
  BaseDeductor* deductor = new IncomeTaxDeductor();

  deductor->SetNext(new NationalInsuranceDeductor());

  const double salary = 25000.0;
  const double net = deductor->Deduct(salary);

  std::cerr << net << std::endl;

  return 0;
}