#include "TaxDatabase.h"

using namespace UKTax;

/*
 * Initialise the singleton instance to null
 */
TaxDatabase* TaxDatabase::instance = nullptr;

IncomeTax TaxDatabase::ParseIncomeTaxData(rapidjson::Value& node)
{
  RAPIDJSON_ASSERT(node.IsObject());

  rapidjson::Value& incomeTaxNode = node["income_tax"];
  RAPIDJSON_ASSERT(incomeTaxNode.IsObject());

  rapidjson::Value& bandsNode = incomeTaxNode["bands"];
  RAPIDJSON_ASSERT(bandsNode.IsArray());

  rapidjson::Value& ratesNode = incomeTaxNode["rates"];
  RAPIDJSON_ASSERT(ratesNode.IsArray());

  std::vector<double> bands;
  std::vector<double> rates;

  RAPIDJSON_ASSERT(bands.size() == rates.size());

  for (rapidjson::SizeType i = 0, n = bandsNode.Size(); i < n; ++i)
  {
    double band = bandsNode[i].GetDouble();
    double rate = ratesNode[i].GetDouble();

    bands.push_back(band);
    rates.push_back(rate);
  }

  rapidjson::Value& noAllowanceNode = incomeTaxNode["no_allowance_threshold"];

  RAPIDJSON_ASSERT(noAllowanceNode.IsDouble());
  double noAllowanceThreshold = noAllowanceNode.GetDouble();

  IncomeTax incomeTax(bands, rates, noAllowanceThreshold);

  return incomeTax;
}

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

    rapidjson::Value& scotlandNode = document["scotland"];
    rapidjson::Value& englandNode = document["england"];

    RAPIDJSON_ASSERT(scotlandNode.IsObject());
    RAPIDJSON_ASSERT(englandNode.IsObject());

    IncomeTax scotlandIncomeTax = ParseIncomeTaxData(scotlandNode);
    IncomeTax englandIncomeTax = ParseIncomeTaxData(englandNode);

    incomeTax.insert(std::pair<UKRegion, IncomeTax>(UKRegion::eScotland, scotlandIncomeTax));
    incomeTax.insert(std::pair<UKRegion, IncomeTax>(UKRegion::eEngland, englandIncomeTax));
    incomeTax.insert(std::pair<UKRegion, IncomeTax>(UKRegion::eWales, englandIncomeTax));
    incomeTax.insert(std::pair<UKRegion, IncomeTax>(UKRegion::eNorthernIreland, englandIncomeTax));

    rapidjson::Value& nationalInsuranceNode = document["national_insurance"];
    rapidjson::Value& studentLoanNode = document["student_loan"];

    RAPIDJSON_ASSERT(nationalInsuranceNode.IsObject());
    RAPIDJSON_ASSERT(studentLoanNode.IsObject());

    if (nationalInsuranceNode.IsObject() &&
        studentLoanNode.IsObject())
    {
      rapidjson::Value& niBandsNode = nationalInsuranceNode["bands"];
      rapidjson::Value& niRatesNode = nationalInsuranceNode["rates"];

      RAPIDJSON_ASSERT(niBandsNode.Size() == niRatesNode.Size());

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

    using Pair = std::pair<Threshold, Rate>;
    using Mapping = std::pair<StudentLoanPlan, std::pair<Threshold, Rate>>;

    if (plan1Node.IsObject() &&
        plan2Node.IsObject() &&
        plan4Node.IsObject() &&
        planPostGradNode.IsObject())
    {
      Pair data = std::make_pair<Threshold, Rate>(
        plan1Node["threshold"].GetDouble(),
        plan1Node["rate"].GetDouble()
      );

      studentLoansData.insert(Mapping(StudentLoanPlan::ePlan1, data));

      data = std::make_pair<Threshold, Rate>(
        plan2Node["threshold"].GetDouble(),
        plan2Node["rate"].GetDouble()
      );

      studentLoansData.insert(Mapping(StudentLoanPlan::ePlan2, data));

      data = std::make_pair<Threshold, Rate>(
        plan4Node["threshold"].GetDouble(),
        plan4Node["rate"].GetDouble()
      );

      studentLoansData.insert(Mapping(StudentLoanPlan::ePlan4, data));

      data = std::make_pair<Threshold, Rate>(
        planPostGradNode["threshold"].GetDouble(),
        planPostGradNode["rate"].GetDouble()
      );

      studentLoansData.insert(Mapping(StudentLoanPlan::ePostGrad, data));
    }

    haveData = true;
  }
}

std::vector<double> TaxDatabase::GetData(TaxDataType type, UKRegion region)
{
  if (!haveData)
    ReadData();

  std::map<UKRegion, IncomeTax>::iterator iter;
  std::vector<double> emptyResult;

  switch (type)
  {
  default:
  case TaxDataType::eNIRates:
    return nationalInsuranceRates;

  case TaxDataType::eNIBands:
    return nationalInsuranceBands;

  case TaxDataType::eITRates:

    iter = incomeTax.find(region);
    if (iter != incomeTax.end())
    {
      IncomeTax tax = iter->second;
      return tax.GetRates();
    }

    return emptyResult;

  case TaxDataType::eITBands:

    iter = incomeTax.find(region);
    if (iter != incomeTax.end())
    {
      IncomeTax tax = iter->second;
      return tax.GetBands();
    }

    return emptyResult;
  }
}

std::vector<double> TaxDatabase::GetIncomeTaxRates(UKRegion region)
{
  return GetData(TaxDataType::eITRates, region);
}

std::vector<double> TaxDatabase::GetIncomeTaxBands(UKRegion region)
{
  return GetData(TaxDataType::eITBands, region);
}

IncomeTax TaxDatabase::GetIncomeTax(UKRegion region)
{
  std::map<UKRegion, IncomeTax>::iterator iter = incomeTax.find(region);
  if (iter != incomeTax.end())
    return iter->second;

  throw std::exception();
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

  std::map<StudentLoanPlan, std::pair<Threshold, Rate>>::iterator iter = studentLoansData.find(plan);

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