#pragma once

#include "TaxDatabase.h"

/**
 * Base class for chain of responsibility pattern for
 * deducting different types of tax from an initial amount.
 * 
 * @author Gary Hannah
 */
namespace UKTax
{
  namespace Deductors
  {
    class BaseTaxDeductor
    {
    protected:
      BaseTaxDeductor* next;
      double gross;
      TaxDatabase::UKRegion region;

      double TaxForAmount(double lower, double higher, double rate);
    public:
      BaseTaxDeductor() :
        gross(0.0),
        next(nullptr),
        region(TaxDatabase::UKRegion::eEngland)
      {}

      BaseTaxDeductor* SetNext(BaseTaxDeductor* next)
      {
        this->next = next;
        return next;
      }

      virtual ~BaseTaxDeductor()
      {
        if (next)
          delete next;
      }

      virtual double Deduct(double amount)
      {
        if (next)
          return next->Deduct(amount);

        return amount;
      }
    };
  };
};