#include "TaxDatabase.h"

/*
 * Initialise the singleton instance to null
 */
TaxDatabase* TaxDatabase::instance = nullptr;

void TaxDatabase::ReadData()
{
  std::fstream fs;
  fs.open(resourceFile, std::ios::in);

  if (fs.is_open())
  {
    std::stringstream json;
    std::string line;

    while (std::getline(fs, line))
      json << line;

    /*
     * Parse the data
     */
    rapidjson::Document document;

    document.Parse(json.str().c_str());

    rapidjson::Value& incomeTaxNode = document["income_tax"];
    rapidjson::Value& nationalInsuranceNode = document["national_insurance"];
    rapidjson::Value& studentLoanNode = document["student_loan"];

    RAPIDJSON_ASSERT(incomeTaxNode.IsObject());
    RAPIDJSON_ASSERT(nationalInsuranceNode.IsObject());
    RAPIDJSON_ASSERT(studentLoanNode.IsObject());

    if (incomeTaxNode.IsObject() &&
        nationalInsuranceNode.IsObject() &&
        studentLoanNode.IsObject())
    {
      rapidjson::Value& iBandsNode = incomeTaxNode["bands"];
      rapidjson::Value& iRatesNode = incomeTaxNode["rates"];
      rapidjson::Value& niBandsNode = nationalInsuranceNode["bands"];
      rapidjson::Value& niRatesNode = nationalInsuranceNode["rates"];

      RAPIDJSON_ASSERT(iBandsNode.Size() == iRatesNode.Size());
      RAPIDJSON_ASSERT(niBandsNode.Size() == niRatesNode.Size());

      for (rapidjson::SizeType i = 0, n = iBandsNode.Size(); i < n; ++i)
      {
        const double band = iBandsNode[i].GetDouble();
        const double rate = iRatesNode[i].GetDouble();

        incomeTaxBands.push_back(band);
        incomeTaxRates.push_back(rate);
      }

      for (rapidjson::SizeType i = 0, n = niBandsNode.Size(); i < n; ++i)
      {
        const double band = niBandsNode[i].GetDouble();
        const double rate = niRatesNode[i].GetDouble();

        nationalInsuranceBands.push_back(band);
        nationalInsuranceRates.push_back(rate);
      }
    }

    rapidjson::Value& plan1Node = studentLoanNode["plan1"];
    rapidjson::Value& plan2Node = studentLoanNode["plan2"];
    rapidjson::Value& plan4Node = studentLoanNode["plan4"];
    rapidjson::Value& planPostGradNode = studentLoanNode["postgrad"];

    RAPIDJSON_ASSERT(plan1Node.IsObject());
    RAPIDJSON_ASSERT(plan2Node.IsObject());
    RAPIDJSON_ASSERT(plan4Node.IsObject());
    RAPIDJSON_ASSERT(planPostGradNode.IsObject());

    using SLData = std::pair<Threshold, Rate>;
    using MapPair = std::pair<StudentLoanPlan, std::pair<Threshold, Rate>>;

    if (plan1Node.IsObject() &&
        plan2Node.IsObject() &&
        plan4Node.IsObject() &&
        planPostGradNode.IsObject())
    {
      SLData data = std::make_pair<TaxDatabase::Threshold,TaxDatabase::Rate>(
        plan1Node["threshold"].GetDouble(),
        plan1Node["rate"].GetDouble()
      );

      studentLoansData.insert(MapPair(StudentLoanPlan::ePlan1, data));

      data = std::make_pair<TaxDatabase::Threshold, TaxDatabase::Rate>(
        plan2Node["threshold"].GetDouble(),
        plan2Node["rate"].GetDouble()
      );

      studentLoansData.insert(MapPair(StudentLoanPlan::ePlan2, data));

      data = std::make_pair<TaxDatabase::Threshold, TaxDatabase::Rate>(
        plan4Node["threshold"].GetDouble(),
        plan4Node["rate"].GetDouble()
      );

      studentLoansData.insert(MapPair(StudentLoanPlan::ePlan4, data));

      data = std::make_pair<TaxDatabase::Threshold, TaxDatabase::Rate>(
        planPostGradNode["threshold"].GetDouble(),
        planPostGradNode["rate"].GetDouble()
      );

      studentLoansData.insert(MapPair(StudentLoanPlan::ePostGrad, data));
    }

    haveData = true;
  }
}

std::vector<double> TaxDatabase::GetData(TaxDataType type)
{
  if (!haveData)
    ReadData();

  switch (type)
  {
  default:
  case TaxDataType::eNIRates:
    return nationalInsuranceRates;
  case TaxDataType::eNIBands:
    return nationalInsuranceBands;
  case TaxDataType::eITRates:
    return incomeTaxRates;
  case TaxDataType::eITBands:
    return incomeTaxBands;
  }
}

std::vector<double> TaxDatabase::GetIncomeTaxRates()
{
  return GetData(TaxDataType::eITRates);
}

std::vector<double> TaxDatabase::GetIncomeTaxBands()
{
  return GetData(TaxDataType::eITBands);
}

std::vector<double> TaxDatabase::GetNationalInsuranceRates()
{
  return GetData(TaxDataType::eNIRates);
}

std::vector<double> TaxDatabase::GetNationalInsuranceBands()
{
  return GetData(TaxDataType::eNIBands);
}

std::pair<TaxDatabase::Threshold,TaxDatabase::Rate> TaxDatabase::GetStudentLoanData(StudentLoanPlan plan)
{
  if (!haveData)
    ReadData();

  std::map<StudentLoanPlan, std::pair<double, double>>::iterator iter = studentLoansData.find(plan);

  if (iter != studentLoansData.end())
    return iter->second;

  throw std::exception();
}

double TaxDatabase::GetStudentLoanThreshold(StudentLoanPlan plan)
{
  std::pair<Threshold, Rate> data = GetStudentLoanData(plan);
  return data.first;
}

double TaxDatabase::GetStudentLoanRate(StudentLoanPlan plan)
{
  std::pair<Threshold, Rate> data = GetStudentLoanData(plan);
  return data.second;
}