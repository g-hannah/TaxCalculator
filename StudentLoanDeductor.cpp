#include "StudentLoanDeductor.h"

double StudentLoanDeductor::Deduct(double amount)
{
  TaxDatabase* database = TaxDatabase::GetInstance();
  std::pair<TaxDatabase::Threshold,TaxDatabase::Rate> data = database->GetStudentLoanData(plan);

  TaxDatabase::Threshold threshold = StudentLoanThreshold(data);
  TaxDatabase::Rate rate = StudentLoanRate(data);

  /*
   * Student loan repayment is calculated on the
   * gross salary but taken from net salary...
   */
  if (salary > threshold)
  {
    std::cerr << "\nStudent Loan Tax:\n" << "Total amount taxed: " << (salary - threshold) << "\nTotal tax: " << (rate * (salary - threshold)) << std::endl;
    amount -= (rate * (salary - threshold));
  }

  return BaseDeductor::Deduct(amount);
}