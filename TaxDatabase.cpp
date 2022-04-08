#include "TaxDatabase.h"

/*
 * Initialise the singleton instance to null
 */
TaxDatabase* TaxDatabase::instance = nullptr;

void TaxDatabase::GetData()
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

    RAPIDJSON_ASSERT(incomeTaxNode.IsObject());
    RAPIDJSON_ASSERT(nationalInsuranceNode.IsObject());

    if (incomeTaxNode.IsObject() && nationalInsuranceNode.IsObject())
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

    haveData = true;
  }
}

std::vector<double> TaxDatabase::GetIncomeTaxRates()
{
  if (haveData)
    return incomeTaxRates;

  GetData();

  return incomeTaxRates;
}

std::vector<double> TaxDatabase::GetIncomeTaxBands()
{
  if (haveData)
    return incomeTaxRates;

  GetData();

  return incomeTaxBands;
}

std::vector<double> TaxDatabase::GetNationalInsuranceRates()
{
  if (haveData)
    return nationalInsuranceRates;

  GetData();

  return nationalInsuranceRates;
}

std::vector<double> TaxDatabase::GetNationalInsuranceBands()
{
  if (haveData)
    return nationalInsuranceBands;

  GetData();

  return nationalInsuranceBands;
}