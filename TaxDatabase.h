#pragma once

#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include "IncomeTax.h"
#include "include/rapidjson/rapidjson.h"
#include "include/rapidjson/document.h"
#include "include/rapidjson/stringbuffer.h"

/**
 * Singleton class which handles retrieval and storage
 * of information on various types of taxes.
 * 
 * @author Gary Hannah
 */
namespace UKTax
{
  class TaxDatabase
  {
  private:
    TaxDatabase() { }
    ~TaxDatabase() { }

    static TaxDatabase* instance;
    const std::string resourceFile = "resources/tax_data.json"; // XXX store string constants in external data file
    void ReadData();
    IncomeTax ParseIncomeTaxData(rapidjson::Value&);

    bool haveData = false;

  public:
    enum class UKRegion
    {
      eScotland,
      eEngland,
      eWales,
      eNorthernIreland,
      eAllRegions
    };

  private:

    std::map<UKRegion, IncomeTax> incomeTax;

    std::vector<double> nationalInsuranceBands;
    std::vector<double> nationalInsuranceRates;

    enum class TaxDataType
    {
      eNIRates,
      eNIBands,
      eITRates,
      eITBands
    };

  public:
    enum class StudentLoanPlan
    {
      ePlan1,
      ePlan2,
      ePlan4,
      ePostGrad
    };

  private:
    std::map<StudentLoanPlan, std::pair<double, double>> studentLoansData;

    std::vector<double> GetData(TaxDataType, UKRegion region = UKRegion::eAllRegions);

  public:
    static TaxDatabase* GetInstance()
    {
      if (!instance)
      {
        instance = new TaxDatabase();
      }

      return instance;
    }

    std::vector<double> GetIncomeTaxRates(UKRegion);
    std::vector<double> GetIncomeTaxBands(UKRegion);
    double GetIncomeTaxNoAllowanceThreshold(UKRegion);
    IncomeTax GetIncomeTax(UKRegion);
    std::vector<double> GetNationalInsuranceRates();
    std::vector<double> GetNationalInsuranceBands();

    using Threshold = double;
    using Rate = double;
    std::pair<Threshold, Rate> GetStudentLoanData(StudentLoanPlan);
    double GetStudentLoanThreshold(StudentLoanPlan);
    double GetStudentLoanRate(StudentLoanPlan);

#define StudentLoanThreshold(pair) (pair).first
#define StudentLoanRate(pair) (pair).second
  };
};