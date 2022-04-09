#pragma once

#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include "include/rapidjson/rapidjson.h"
#include "include/rapidjson/document.h"
#include "include/rapidjson/stringbuffer.h"

/**
 * A singleton class which handles retrieval and storage
 * of information various types of tax bands and rates.
 * 
 * @author Gary Hannah
 */
class TaxDatabase
{
private:
  TaxDatabase() { }
  ~TaxDatabase() { }

  static TaxDatabase* instance;
  const std::string resourceFile = "resources/income_tax.json";
  void ReadData();

  bool haveData = false;
  std::vector<double> incomeTaxBands;
  std::vector<double> incomeTaxRates;
  std::vector<double> nationalInsuranceBands;
  std::vector<double> nationalInsuranceRates;

  enum class TaxDataType
  {
    eNIRates,
    eNIBands,
    eITRates,
    eITBands
  };

  enum class StudentLoanPlan; // forward declaration
  std::map<StudentLoanPlan, std::pair<double, double>> studentLoansData;

  std::vector<double> GetData(TaxDataType);

public:
  static TaxDatabase* GetInstance()
  {
    if (!instance)
    {
      instance = new TaxDatabase();
    }

    return instance;
  }

  std::vector<double> GetIncomeTaxRates();
  std::vector<double> GetIncomeTaxBands();
  std::vector<double> GetNationalInsuranceRates();
  std::vector<double> GetNationalInsuranceBands();

  using Threshold = double;
  using Rate = double;
  std::pair<Threshold,Rate> GetStudentLoanData(StudentLoanPlan);
  double GetStudentLoanThreshold(StudentLoanPlan);
  double GetStudentLoanRate(StudentLoanPlan);

  enum class StudentLoanPlan
  {
    ePlan1,
    ePlan2,
    ePlan4,
    ePostGrad
  };

#define StudentLoanThreshold(pair) (pair).first
#define StudentLoanRate(pair) (pair).second
};

