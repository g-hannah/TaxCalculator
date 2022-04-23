#include "TaxDatabase.h"

namespace UKTax
{
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

        TaxValues niBands;
        TaxValues niRates;

        RAPIDJSON_ASSERT(niBandsNode.Size() == niRatesNode.Size());

        for (rapidjson::SizeType i = 0, n = niBandsNode.Size(); i < n; ++i)
        {
          const double band = niBandsNode[i].GetDouble();
          const double rate = niRatesNode[i].GetDouble();

          niBands.push_back(band);
          niRates.push_back(rate);
        }

        NationalInsuranceTax nit(niBands, niRates);

        /*
         * National Insurance tax is the same across regions.
         */
        nationalInsuranceTax.insert(std::pair<UKRegion, NationalInsuranceTax>(UKRegion::eAllRegions, nit));
        nationalInsuranceTax.insert(std::pair<UKRegion, NationalInsuranceTax>(UKRegion::eScotland, nit));
        nationalInsuranceTax.insert(std::pair<UKRegion, NationalInsuranceTax>(UKRegion::eEngland, nit));
        nationalInsuranceTax.insert(std::pair<UKRegion, NationalInsuranceTax>(UKRegion::eWales, nit));
        nationalInsuranceTax.insert(std::pair<UKRegion, NationalInsuranceTax>(UKRegion::eNorthernIreland, nit));
      }

      rapidjson::Value& plan1Node = studentLoanNode["plan1"];
      rapidjson::Value& plan2Node = studentLoanNode["plan2"];
      rapidjson::Value& plan4Node = studentLoanNode["plan4"];
      rapidjson::Value& planPostGradNode = studentLoanNode["postgrad"];

      RAPIDJSON_ASSERT(plan1Node.IsObject());
      RAPIDJSON_ASSERT(plan2Node.IsObject());
      RAPIDJSON_ASSERT(plan4Node.IsObject());
      RAPIDJSON_ASSERT(planPostGradNode.IsObject());

      using StudentLoanMapping = std::pair<StudentLoanPlan, StudentLoanTax>;

      if (plan1Node.IsObject() &&
        plan2Node.IsObject() &&
        plan4Node.IsObject() &&
        planPostGradNode.IsObject())
      {
        StudentLoanTax studentLoanTaxPlan1(plan1Node["threshold"].GetDouble(), plan1Node["rate"].GetDouble());
        StudentLoanTax studentLoanTaxPlan2(plan2Node["threshold"].GetDouble(), plan2Node["rate"].GetDouble());
        StudentLoanTax studentLoanTaxPlan4(plan4Node["threshold"].GetDouble(), plan4Node["rate"].GetDouble());
        StudentLoanTax studentLoanTaxPlanPostgrad(planPostGradNode["threshold"].GetDouble(), planPostGradNode["rate"].GetDouble());

        studentLoanTax.insert(StudentLoanMapping(StudentLoanPlan::ePlan1, studentLoanTaxPlan1));
        studentLoanTax.insert(StudentLoanMapping(StudentLoanPlan::ePlan2, studentLoanTaxPlan2));
        studentLoanTax.insert(StudentLoanMapping(StudentLoanPlan::ePlan4, studentLoanTaxPlan4));
        studentLoanTax.insert(StudentLoanMapping(StudentLoanPlan::ePostGrad, studentLoanTaxPlanPostgrad));
      }

      haveData = true;
    }
  }

  std::any TaxDatabase::GetData(TaxDataType type, std::any key)
  {
    if (!haveData)
      ReadData();

    std::map<UKRegion, NationalInsuranceTax>::iterator niIter;
    std::map<UKRegion, IncomeTax>::iterator itIter;
    std::map<StudentLoanPlan, StudentLoanTax>::iterator sltIter;

    switch (type)
    {
    case TaxDataType::eNationalInsurance:

      niIter = nationalInsuranceTax.find(std::any_cast<UKRegion>(key));
      if (niIter != nationalInsuranceTax.end())
        return std::any(niIter->second);
      break;

    case TaxDataType::eIncomeTax:

      itIter = incomeTax.find(std::any_cast<UKRegion>(key));
      if (itIter != incomeTax.end())
        return std::any(itIter->second);
      break;

    case TaxDataType::eStudentLoanTax:

      sltIter = studentLoanTax.find(std::any_cast<StudentLoanPlan>(key));
      if (sltIter != studentLoanTax.end())
        return std::any(sltIter->second);
      break;

    default:
      break;
    }

    throw std::exception("No data found for the given type.");
  }

  IncomeTax TaxDatabase::GetIncomeTax(UKRegion region)
  {
    return std::any_cast<IncomeTax>(GetData(TaxDataType::eIncomeTax, std::any(region)));
  }

  StudentLoanTax TaxDatabase::GetStudentLoanTax(StudentLoanPlan plan)
  {
    return std::any_cast<StudentLoanTax>(GetData(TaxDataType::eStudentLoanTax, std::any(plan)));
  }

  NationalInsuranceTax TaxDatabase::GetNationalInsuranceTax(UKRegion region)
  {
    return std::any_cast<NationalInsuranceTax>(GetData(TaxDataType::eNationalInsurance, std::any(region)));
  }
};