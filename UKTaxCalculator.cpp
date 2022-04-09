#include <iostream>
#include "IncomeTaxDeductor.h"
#include "NationalInsuranceDeductor.h"
#include "StudentLoanDeductor.h"
#include "include/rapidjson/document.h"
#include "include/rapidjson/stringbuffer.h"

static double GetSalary()
{
  double salary = 0.0;

  std::fstream fs;

  fs.open("resources/salary.json");
  if (fs.is_open())
  {
    std::string line;
    std::stringstream ss;

    while (std::getline(fs, line))
    {
      ss << line;
    }

    rapidjson::Document document;

    document.Parse(ss.str().c_str());
    rapidjson::Value& node = document["salary"];

    RAPIDJSON_ASSERT(node.IsDouble());
    salary = node.GetDouble();
  }

  return salary;
}

int main()
{
  const double salary = GetSalary();
  UKTax::Deductors::BaseDeductor* deductor = new UKTax::Deductors::NationalInsuranceDeductor();

  deductor
    ->SetNext(new UKTax::Deductors::IncomeTaxDeductor(UKTax::TaxDatabase::UKRegion::eScotland))
    ->SetNext(new UKTax::Deductors::StudentLoanDeductor(UKTax::TaxDatabase::StudentLoanPlan::ePlan4, salary));

  std::cerr << "\nGross Salary\n" << salary << std::endl;

  const double net = deductor->Deduct(salary);

  std::cerr << "\nNet Salary\n" << net << std::endl;

  delete deductor;

  return 0;
}