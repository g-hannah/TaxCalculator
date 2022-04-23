#include <iostream>
#include <memory>
#include "DeductorBuilder.h"
#include "include/rapidjson/document.h"
#include "include/rapidjson/stringbuffer.h"

static double GetSalary()
{
  double gross = 0.0;

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
    gross = node.GetDouble();
  }

  return gross;
}

int main()
{
  const double gross = GetSalary();

  try
  {
    std::unique_ptr<UKTax::Deductors::DeductorBuilder> builder = std::make_unique<UKTax::Deductors::DeductorBuilder>();

    std::unique_ptr<UKTax::Deductors::BaseTaxDeductor> deductor = builder
      ->SetGross(gross)
      ->SetRegion(UKTax::TaxDatabase::UKRegion::eScotland)
      ->SetStudentLoanPlan(UKTax::TaxDatabase::StudentLoanPlan::ePlan4)
      ->Build();

    std::cerr << "\nGross Salary\n" << gross << " GBP" << std::endl;

    const double net = deductor->Deduct(gross);

    std::cerr << "\nNet Salary" << std::endl;
    std::cerr << " Yearly: " << net << " GBP" << std::endl;
    std::cerr << "Monthly: " << (net / 12.0) << " GBP" << std::endl;
    std::cerr << " Weekly: " << (net / 52.0) << " GBP" << std::endl;
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}