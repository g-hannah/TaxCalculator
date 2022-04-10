#include <iostream>
#include <memory>
#include "DeductorBuilder.h"
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

  std::unique_ptr<UKTax::Deductors::DeductorBuilder> builder = std::make_unique<UKTax::Deductors::DeductorBuilder>();

  std::unique_ptr<UKTax::Deductors::BaseDeductor> deductor = builder
    ->SetSalary(salary)
    ->SetRegion(UKTax::TaxDatabase::UKRegion::eScotland)
    ->SetStudentLoanPlan(UKTax::TaxDatabase::StudentLoanPlan::ePlan4)
    ->Build();

  std::cerr << "\nGross Salary\n" << salary << std::endl;

  const double net = deductor->Deduct(salary);

  std::cerr << "\nNet Salary\n" << net << std::endl;

  return 0;
}