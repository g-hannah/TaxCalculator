#pragma once

#include <vector>
#include <iostream>

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
  void GetData();

  bool haveData = false;
  std::vector<double> incomeTaxBands;
  std::vector<double> incomeTaxRates;
  std::vector<double> nationalInsuranceBands;
  std::vector<double> nationalInsuranceRates;

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
};

