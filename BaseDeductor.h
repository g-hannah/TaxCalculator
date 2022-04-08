#pragma once

#include "TaxDatabase.h"

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