#include "errno.h"
#include "stdlib.h"

int const costPerCredit = 200;
int const totalCredits = 120;
int const maxSemesterCredits = 20;
int id = 0;
int passedCredits = 0;
int enrollmentCredits = 0;
int tuitionBalance = 0;
_Bool lateRegistration = (_Bool)0;
void init_StudentEnrollment(int newId);
void setTuitionBalance(int balance);
void setPassedCredits(int credits);
void setEnrollmentCredits(int credits);
void setLateRegistration(_Bool isLate);
int getTuition(void);
int getEnrollmentCredits(void);
int getPassedCredits(void);
_Bool getLateRegistrations(void);
int getId(void);
_Bool registrationPermission(void);
_Bool validPayment(int payment);
void regularEnrollment(void);
void lateEnrollment(void);
void enroll(void);
void payTuitionWithDebitCard(int payment);
void payTuitionWithCreditCard(int payment);
void pay(int payment, _Bool debit);
void enrollmentProcess(int pCredits, int sCredits, int payment,
                       int initBalance, _Bool isLate, _Bool debit,
                       _Bool option);
int main(void);

/*@
assigns id, passedCredits, enrollmentCredits, tuitionBalance;
ensures id == newId;
ensures passedCredits == 0;
ensures enrollmentCredits == 0;
ensures tuitionBalance == 0;
*/
void init_StudentEnrollment(int newId)
{
  id = newId;
  passedCredits = 0;
  enrollmentCredits = 0;
  tuitionBalance = 0;
  return;
}

/*@
requires maxSemesterCredits == 20;
requires costPerCredit == 200;
assigns tuitionBalance;
ensures balance <= 4240 ==> tuitionBalance == balance;
*/
void setTuitionBalance(int balance)
{
  /*@ assert
        rte_2: signed_overflow:
        -2147483648 ≤ maxSemesterCredits * costPerCredit;
  */
  /*@ assert
        rte_3: signed_overflow: maxSemesterCredits * costPerCredit ≤ 2147483647;
  */
  /*@ assert
        rte_4: signed_overflow: -2147483648 ≤ (int)(costPerCredit / 100) * 6;
  */
  /*@ assert
        rte_5: signed_overflow: (int)(costPerCredit / 100) * 6 ≤ 2147483647;
  */
  /*@ assert
        rte_6: signed_overflow:
        -2147483648 ≤
        maxSemesterCredits * (int)((int)(costPerCredit / 100) * 6);
  */
  /*@ assert
        rte_7: signed_overflow:
        maxSemesterCredits * (int)((int)(costPerCredit / 100) * 6) ≤
        2147483647;
  */
  /*@ assert
        rte_8: signed_overflow:
        -2147483648 ≤
        (int)(maxSemesterCredits * costPerCredit) +
        (int)(maxSemesterCredits * (int)((int)(costPerCredit / 100) * 6));
  */
  /*@ assert
        rte_9: signed_overflow:
        (int)(maxSemesterCredits * costPerCredit) +
        (int)(maxSemesterCredits * (int)((int)(costPerCredit / 100) * 6)) ≤
        2147483647;
  */
  int maxTuition =
    maxSemesterCredits * costPerCredit + maxSemesterCredits * ((costPerCredit / 100) * 6);
  if (balance > maxTuition) exit(-1);
  tuitionBalance = balance;
  return;
}

/*@
assigns passedCredits;
ensures passedCredits == credits;
ensures 0 <= passedCredits <= totalCredits;
*/
void setPassedCredits(int credits)
{
  if (credits < 0) goto _LOR;
  else 
    if (credits > totalCredits) _LOR: exit(-1);
  passedCredits = credits;
  return;
}

/*@
requires 0 <= credits <= maxSemesterCredits;
assigns enrollmentCredits;
ensures enrollmentCredits == credits;
*/
void setEnrollmentCredits(int credits)
{
  if (credits < 0) goto _LOR;
  else 
    if (credits > maxSemesterCredits) _LOR: exit(-1);
  enrollmentCredits = credits;
  return;
}

/*@
assigns lateRegistration;
ensures lateRegistration == isLate;
*/
void setLateRegistration(_Bool isLate)
{
  lateRegistration = isLate;
  return;
}

/*@
assigns \nothing;
ensures \result == tuitionBalance;
*/
int getTuition(void)
{
  return tuitionBalance;
}

/*@
assigns \nothing;
ensures \result == enrollmentCredits;
*/
int getEnrollmentCredits(void)
{
  return enrollmentCredits;
}

/*@
assigns \nothing;
ensures \result == passedCredits;
*/
int getPassedCredits(void)
{
  return passedCredits;
}

/*@
assigns \nothing;
ensures \result == 0 || \result == 1;
ensures \result == lateRegistration;
*/
_Bool getLateRegistrations(void)
{
  /*@ assert
        rte_1: bool_value: lateRegistration ≡ 0 ∨ lateRegistration ≡ 1;
  */
  return lateRegistration;
}

/*@
assigns \nothing;
ensures \result == id;
*/
int getId(void)
{
  return id;
}

/*@
assigns \nothing;
ensures \result == (tuitionBalance <= 0);
*/
_Bool registrationPermission(void)
{
  _Bool __retres;
  __retres = (_Bool)(tuitionBalance <= 0);
  return __retres;
}

/*@
assigns \nothing;
ensures \result == 1 || \result == 0;
ensures (payment > 0) ==> (\result == 1);
ensures (payment <= 0) ==> (\result == 0);
*/
_Bool validPayment(int payment)
{
  _Bool __retres;
  __retres = (_Bool)(payment > 0);
  return __retres;
}

/*@
requires tuitionBalance <= 0;
requires enrollmentCredits >= 0;
requires enrollmentCredits * costPerCredit <= 2147483647;
requires tuitionBalance + enrollmentCredits * costPerCredit >= -2147483648;
requires tuitionBalance + enrollmentCredits * costPerCredit <= 2147483647;
assigns tuitionBalance;
ensures tuitionBalance == \old(tuitionBalance) + \old(enrollmentCredits) * costPerCredit;
*/
void regularEnrollment(void)
{
  if (! (tuitionBalance <= 0)) exit(-1);
  /*@ assert
        rte_20: signed_overflow: -2147483648 ≤ enrollmentCredits * costPerCredit;
  */
  /*@ assert
        rte_21: signed_overflow: enrollmentCredits * costPerCredit ≤ 2147483647;
  */
  /*@ assert
        rte_22: signed_overflow:
        -2147483648 ≤
        tuitionBalance + (int)(enrollmentCredits * costPerCredit);
  */
  /*@ assert
        rte_23: signed_overflow:
        tuitionBalance + (int)(enrollmentCredits * costPerCredit) ≤
        2147483647;
  */
  tuitionBalance += enrollmentCredits * costPerCredit;
  return;
}

/*@
requires tuitionBalance <= 0;
requires enrollmentCredits >= 0;
requires enrollmentCredits <= 10000000;
requires costPerCredit == 200;
assigns tuitionBalance;
ensures tuitionBalance == \old(tuitionBalance) + enrollmentCredits * costPerCredit + enrollmentCredits * ((costPerCredit / 100) * 6);
*/
void lateEnrollment(void)
{
  if (! (tuitionBalance <= 0)) exit(-1);
  /*@ assert
        rte_10: signed_overflow: -2147483648 ≤ (int)(costPerCredit / 100) * 6;
  */
  /*@ assert
        rte_11: signed_overflow: (int)(costPerCredit / 100) * 6 ≤ 2147483647;
  */
  int latePerCredit = (costPerCredit / 100) * 6;
  /*@ assert
        rte_12: signed_overflow: -2147483648 ≤ enrollmentCredits * costPerCredit;
  */
  /*@ assert
        rte_13: signed_overflow: enrollmentCredits * costPerCredit ≤ 2147483647;
  */
  /*@ assert
        rte_14: signed_overflow: -2147483648 ≤ enrollmentCredits * latePerCredit;
  */
  /*@ assert
        rte_15: signed_overflow: enrollmentCredits * latePerCredit ≤ 2147483647;
  */
  /*@ assert
        rte_16: signed_overflow:
        -2147483648 ≤
        (int)(enrollmentCredits * costPerCredit) +
        (int)(enrollmentCredits * latePerCredit);
  */
  /*@ assert
        rte_17: signed_overflow:
        (int)(enrollmentCredits * costPerCredit) +
        (int)(enrollmentCredits * latePerCredit) ≤ 2147483647;
  */
  /*@ assert
        rte_18: signed_overflow:
        -2147483648 ≤
        tuitionBalance +
        (int)((int)(enrollmentCredits * costPerCredit) +
              (int)(enrollmentCredits * latePerCredit));
  */
  /*@ assert
        rte_19: signed_overflow:
        tuitionBalance +
        (int)((int)(enrollmentCredits * costPerCredit) +
              (int)(enrollmentCredits * latePerCredit))
        ≤ 2147483647;
  */
  tuitionBalance += enrollmentCredits * costPerCredit + enrollmentCredits * latePerCredit;
  return;
}

/*@
requires lateRegistration == 0 || lateRegistration == 1;
requires tuitionBalance <= 0;
requires enrollmentCredits >= 0;
requires enrollmentCredits <= 10000000;
requires costPerCredit == 200;
requires enrollmentCredits * costPerCredit <= 2147483647;
requires tuitionBalance + enrollmentCredits * costPerCredit >= -2147483648;
requires tuitionBalance + enrollmentCredits * costPerCredit <= 2147483647;
assigns tuitionBalance;
*/
void enroll(void)
{
  /*@ assert
        rte_24: bool_value: lateRegistration ≡ 0 ∨ lateRegistration ≡ 1;
  */
  if (lateRegistration) lateEnrollment(); else regularEnrollment();
  return;
}

/*@
requires payment > 0;
requires tuitionBalance - payment >= -2147483648;
assigns tuitionBalance;
ensures tuitionBalance == \old(tuitionBalance) - payment;
*/
void payTuitionWithDebitCard(int payment)
{
  _Bool tmp;
  tmp = validPayment(payment);
  if (! tmp) exit(-1);
  /*@ assert rte_25: signed_overflow: -2147483648 ≤ tuitionBalance - payment;
  */
  /*@ assert rte_26: signed_overflow: tuitionBalance - payment ≤ 2147483647;
  */
  tuitionBalance -= payment;
  return;
}

/*@
requires payment >= 0;
requires tuitionBalance >= 0;
requires tuitionBalance <= 2147483647 - payment;
assigns tuitionBalance;
ensures tuitionBalance < \old(tuitionBalance);
*/
void payTuitionWithCreditCard(int payment)
{
  _Bool tmp;
  /*@ assert rte_27: signed_overflow: -2147483648 ≤ (int)(payment / 100) * 2;
  */
  /*@ assert rte_28: signed_overflow: (int)(payment / 100) * 2 ≤ 2147483647;
  */
  int penalty = (payment / 100) * 2;
  /*@ assert rte_29: signed_overflow: -2147483648 ≤ payment - penalty; */
  /*@ assert rte_30: signed_overflow: payment - penalty ≤ 2147483647; */
  int actualPayment = payment - penalty;
  tmp = validPayment(actualPayment);
  if (! tmp) exit(-1);
  /*@ assert
        rte_31: signed_overflow: -2147483648 ≤ tuitionBalance - actualPayment;
  */
  /*@ assert
        rte_32: signed_overflow: tuitionBalance - actualPayment ≤ 2147483647;
  */
  tuitionBalance -= actualPayment;
  return;
}

/*@
requires debit ==> payment > 0;
requires debit ==> tuitionBalance - payment >= -2147483648;
requires !debit ==> payment >= 0;
requires !debit ==> tuitionBalance >= 0;
requires !debit ==> tuitionBalance <= 2147483647 - payment;
assigns tuitionBalance;
*/
void pay(int payment, _Bool debit)
{
  if (debit) payTuitionWithDebitCard(payment);
  else payTuitionWithCreditCard(payment);
  return;
}

/*@
requires maxSemesterCredits == 20;
requires costPerCredit == 200;
requires totalCredits == 120;
requires debit ==> payment > 0;
requires debit ==> initBalance - payment >= -2147483648;
requires !debit ==> payment >= 0;
requires !debit ==> initBalance >= 0;
requires !debit ==> initBalance <= 2147483647 - payment;
requires option ==> initBalance <= 0;
assigns tuitionBalance, passedCredits, enrollmentCredits, lateRegistration;
*/
void enrollmentProcess(int pCredits, int sCredits, int payment,
                       int initBalance, _Bool isLate, _Bool debit,
                       _Bool option)
{
  /*@ assert
        rte_33: signed_overflow:
        -2147483648 ≤ maxSemesterCredits * costPerCredit;
  */
  /*@ assert
        rte_34: signed_overflow: maxSemesterCredits * costPerCredit ≤ 2147483647;
  */
  /*@ assert
        rte_35: signed_overflow: -2147483648 ≤ (int)(costPerCredit / 100) * 6;
  */
  /*@ assert
        rte_36: signed_overflow: (int)(costPerCredit / 100) * 6 ≤ 2147483647;
  */
  /*@ assert
        rte_37: signed_overflow:
        -2147483648 ≤
        maxSemesterCredits * (int)((int)(costPerCredit / 100) * 6);
  */
  /*@ assert
        rte_38: signed_overflow:
        maxSemesterCredits * (int)((int)(costPerCredit / 100) * 6) ≤
        2147483647;
  */
  /*@ assert
        rte_39: signed_overflow:
        -2147483648 ≤
        (int)(maxSemesterCredits * costPerCredit) +
        (int)(maxSemesterCredits * (int)((int)(costPerCredit / 100) * 6));
  */
  /*@ assert
        rte_40: signed_overflow:
        (int)(maxSemesterCredits * costPerCredit) +
        (int)(maxSemesterCredits * (int)((int)(costPerCredit / 100) * 6)) ≤
        2147483647;
  */
  int maxTuition =
    maxSemesterCredits * costPerCredit + maxSemesterCredits * ((costPerCredit / 100) * 6);
  if (initBalance > maxTuition) goto _LOR;
  else 
    if (pCredits < 0) goto _LOR;
    else 
      if (pCredits > totalCredits) goto _LOR;
      else 
        if (sCredits < 0) goto _LOR;
        else 
          if (sCredits > maxSemesterCredits) _LOR: exit(-1);
  setTuitionBalance(initBalance);
  setPassedCredits(pCredits);
  setEnrollmentCredits(sCredits);
  setLateRegistration(isLate);
  if (option) enroll(); else pay(payment,debit);
  return;
}

int main(void)
{
  int __retres;
  int newId = 114514;
  init_StudentEnrollment(newId);
  enrollmentProcess(30,15,0,0,(_Bool)1,(_Bool)0,(_Bool)1);
  /*@ assert tuitionBalance ≡ 3180; */ ;
  __retres = 0;
  return __retres;
}