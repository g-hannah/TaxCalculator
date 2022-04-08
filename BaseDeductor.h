#pragma once

#include "TaxDatabase.h"

/**
 * Base class for chain of responsibility pattern for
 * deducting different types of tax from an initial amount.
 * 
 * @author Gary Hannah
 */
class BaseDeductor
{
protected:
  BaseDeductor* next;
public:
  BaseDeductor() : next(nullptr) { }

  BaseDeductor* SetNext(BaseDeductor* next)
  {
    this->next = next;
    return next;
  }

  virtual ~BaseDeductor()
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