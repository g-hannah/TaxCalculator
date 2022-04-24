#pragma once

#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <any>
#include "IncomeTax.h"
#include "StudentLoanTax.h"
#include "NationalInsuranceTax.h"
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
    TaxDatabase()
    {}

    ~TaxDatabase()
    {
      delete instance;
      instance = nullptr;
    }

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

    enum class StudentLoanPlan
    {
      ePlan1,
      ePlan2,
      ePlan4,
      ePostGrad
    };

  private:

    std::map<UKRegion, IncomeTax> incomeTax;
    std::map<StudentLoanPlan, StudentLoanTax> studentLoanTax;
    std::map<UKRegion, NationalInsuranceTax> nationalInsuranceTax;

    TaxValues nationalInsuranceBands;
    TaxValues nationalInsuranceRates;

    enum class TaxDataType
    {
      eNationalInsurance,
      eIncomeTax,
      eStudentLoanTax
    };

  private:
    std::any GetData(TaxDataType type, std::any key);

  public:
    static TaxDatabase* Instance()
    {
      if (!instance)
      {
        instance = new TaxDatabase();
      }

      return instance;
    }

    IncomeTax GetIncomeTax(UKRegion);
    NationalInsuranceTax GetNationalInsuranceTax(UKRegion);
    StudentLoanTax GetStudentLoanTax(StudentLoanPlan);
  };
};
