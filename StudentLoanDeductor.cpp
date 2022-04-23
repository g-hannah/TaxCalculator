#include "StudentLoanDeductor.h"

namespace UKTax
{
  namespace Deductors
  {
    double StudentLoanDeductor::Deduct(double amount)
    {
      if (0.0 >= amount)
        return amount;

      TaxDatabase* database = TaxDatabase::GetInstance();

      const StudentLoanTax& studentLoanTax = database->GetStudentLoanTax(this->plan);
      const double band = studentLoanTax.GetBand();
      const double rate = studentLoanTax.GetRate();

      /*
       * Student loan repayment is calculated on the
       * gross salary but taken from net salary...
       */
      if (gross > band)
      {
        std::cerr << "\nStudent Loan Tax:\n" << "Total amount taxed: " << (gross - band) << "\nTotal tax: " << (rate * (gross - band)) << std::endl;
        amount -= BaseTaxDeductor::TaxForAmount(band, gross, rate);
      }

      return BaseTaxDeductor::Deduct(amount);
    }
  };
};