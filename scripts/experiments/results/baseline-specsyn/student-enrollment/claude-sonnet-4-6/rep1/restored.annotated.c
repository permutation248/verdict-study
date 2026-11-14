#include <errno.h>

#include <stdlib.h>

int const costPerCredit = 200;

int const totalCredits = 120;

int const maxSemesterCredits = 20;

int id = 0;

int passedCredits = 0;

int enrollmentCredits = 0;

int tuitionBalance = 0;

_Bool lateRegistration = (_Bool)0;

/*@
	requires newId > 0;
	assigns id, passedCredits, enrollmentCredits, tuitionBalance;
	ensures newId > 0 ==> id == newId;
	ensures newId > 0 ==> passedCredits == 0;
	ensures newId > 0 ==> enrollmentCredits == 0;
	ensures newId > 0 ==> tuitionBalance == 0;
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
	requires costPerCredit == 200;
	requires maxSemesterCredits == 20;
	requires balance >= 0;
	requires balance <= maxSemesterCredits * costPerCredit + maxSemesterCredits * ((costPerCredit / 100) * 6);
	assigns tuitionBalance;
	ensures (balance <= maxSemesterCredits * costPerCredit +maxSemesterCredits * ((costPerCredit / 100) * 6)) ==> (tuitionBalance == balance);
*/
void setTuitionBalance(int balance)
{
/*@ assert rte_2: signed_overflow: -2147483648 <= maxSemesterCredits * costPerCredit; */
/*@ assert rte_3: signed_overflow: maxSemesterCredits * costPerCredit <= 2147483647; */
/*@ assert rte_4: signed_overflow: -2147483648 <= (int)(costPerCredit / 100) * 6; */
/*@ assert rte_5: signed_overflow: (int)(costPerCredit / 100) * 6 <= 2147483647; */
/*@ assert rte_6: signed_overflow: -2147483648 <= maxSemesterCredits * (int)((int)(costPerCredit / 100) * 6); */
/*@ assert rte_7: signed_overflow: maxSemesterCredits * (int)((int)(costPerCredit / 100) * 6) <= 2147483647; */
/*@ assert rte_8: signed_overflow: -2147483648 <= (int)(maxSemesterCredits * costPerCredit) + (int)(maxSemesterCredits * (int)((int)(costPerCredit / 100) * 6)); */
/*@ assert rte_9: signed_overflow: (int)(maxSemesterCredits * costPerCredit) + (int)(maxSemesterCredits * (int)((int)(costPerCredit / 100) * 6)) <= 2147483647; */
	int maxTuition =
	maxSemesterCredits * costPerCredit + maxSemesterCredits * ((costPerCredit / 100) * 6);
	if (balance > maxTuition) exit(-1);
	tuitionBalance = balance;
	return;
}

/*@
	requires 0 <= credits <= totalCredits;
	assigns passedCredits;
	ensures (credits >= 0 && credits <= totalCredits) ==>passedCredits == credits;
	ensures (credits >= 0 && credits <= totalCredits) ==>passedCredits >= 0;
	ensures (credits >= 0 && credits <= totalCredits) ==>passedCredits <= totalCredits;
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
	assigns enrollmentCredits \from credits;
	ensures (credits < 0 || credits > maxSemesterCredits) ==> \false;
	ensures (0 <= credits <= maxSemesterCredits) ==> enrollmentCredits == credits;
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
	requires isLate == (_Bool)0 || isLate == (_Bool)1;
	assigns lateRegistration;
	ensures (isLate == (_Bool)1) ==> (lateRegistration == (_Bool)1);
	ensures (isLate == (_Bool)0) ==> (lateRegistration == (_Bool)0);
	ensures lateRegistration == isLate;
*/
void setLateRegistration(_Bool isLate)
{
	lateRegistration = isLate;
	return;
}

/*@
	requires 0 <= tuitionBalance <= totalCredits * costPerCredit;
	assigns \nothing;
	ensures \result == tuitionBalance;
	ensures 0 <= tuitionBalance ==> \result >= 0;
	ensures tuitionBalance <= totalCredits * costPerCredit ==> \result <= totalCredits * costPerCredit;
*/
int getTuition(void)
{
	return tuitionBalance;
}

/*@
	requires 0 <= enrollmentCredits <= maxSemesterCredits;
	assigns \nothing;
	ensures \result == enrollmentCredits;
	ensures 0 <= enrollmentCredits ==> \result >= 0;
	ensures enrollmentCredits <= maxSemesterCredits ==> \result <= maxSemesterCredits;
*/
int getEnrollmentCredits(void)
{
	return enrollmentCredits;
}

/*@
	requires 0 <= passedCredits <= totalCredits;
	assigns \nothing;
	ensures \result == passedCredits;
	ensures (passedCredits >= 0 && passedCredits <= totalCredits) ==>(\result >= 0 && \result <= totalCredits);
	ensures (passedCredits < 0) ==> (\result < 0);
	ensures (passedCredits > totalCredits) ==> (\result > totalCredits);
*/
int getPassedCredits(void)
{
	return passedCredits;
}

/*@
	requires lateRegistration == 0 || lateRegistration == 1;
	assigns \nothing;
	ensures (lateRegistration == 0) ==> (\result == 0);
	ensures (lateRegistration == 1) ==> (\result == 1);
	ensures \result == 0 || \result == 1;
	ensures \result == lateRegistration;
*/
_Bool getLateRegistrations(void)
{
/*@ assert rte_1: bool_value: lateRegistration == 0 || lateRegistration == 1; */
	return lateRegistration;
}

/*@
	requires \valid_read(&id);
	assigns \nothing;
	ensures \result == \old(id);
*/
int getId(void)
{
	return id;
}

/*@
	requires INT_MIN <= tuitionBalance <= INT_MAX;
	assigns \nothing;
	ensures tuitionBalance <= 0 ==> \result == (_Bool)1;
	ensures tuitionBalance > 0 ==> \result == (_Bool)0;
*/
_Bool registrationPermission(void)
{
	_Bool __retres;
	__retres = (_Bool)(tuitionBalance <= 0);
	return __retres;
}

/*@
	requires \true;
	assigns \nothing;
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
	requires -2147483648 <= tuitionBalance + enrollmentCredits * costPerCredit <= 2147483647;
	requires 0 <= enrollmentCredits <= maxSemesterCredits;
	requires tuitionBalance <= 0;
	assigns tuitionBalance;
	assigns tuitionBalance, enrollmentCredits, passedCredits, id, lateRegistration;
*/
void regularEnrollment(void)
{
	if (! (tuitionBalance <= 0)) exit(-1);
/*@ assert rte_20: signed_overflow: -2147483648 <= enrollmentCredits * costPerCredit; */
/*@ assert rte_21: signed_overflow: enrollmentCredits * costPerCredit <= 2147483647; */
/*@ assert rte_22: signed_overflow: -2147483648 <= tuitionBalance + (int)(enrollmentCredits * costPerCredit); */
/*@ assert rte_23: signed_overflow: tuitionBalance + (int)(enrollmentCredits * costPerCredit) <= 2147483647; */
	tuitionBalance += enrollmentCredits * costPerCredit;
	return;
}

/*@
	requires tuitionBalance <= 0;
	requires 0 <= enrollmentCredits <= maxSemesterCredits;
	requires tuitionBalance >= -2147483648 + enrollmentCredits * (costPerCredit + costPerCredit / 100 * 6);
	assigns tuitionBalance;
*/
void lateEnrollment(void)
{
	if (! (tuitionBalance <= 0)) exit(-1);
/*@ assert rte_10: signed_overflow: -2147483648 <= (int)(costPerCredit / 100) * 6; */
/*@ assert rte_11: signed_overflow: (int)(costPerCredit / 100) * 6 <= 2147483647; */
	int latePerCredit = (costPerCredit / 100) * 6;
/*@ assert rte_12: signed_overflow: -2147483648 <= enrollmentCredits * costPerCredit; */
/*@ assert rte_13: signed_overflow: enrollmentCredits * costPerCredit <= 2147483647; */
/*@ assert rte_14: signed_overflow: -2147483648 <= enrollmentCredits * latePerCredit; */
/*@ assert rte_15: signed_overflow: enrollmentCredits * latePerCredit <= 2147483647; */
/*@ assert rte_16: signed_overflow: -2147483648 <= (int)(enrollmentCredits * costPerCredit) + (int)(enrollmentCredits * latePerCredit); */
/*@ assert rte_17: signed_overflow: (int)(enrollmentCredits * costPerCredit) + (int)(enrollmentCredits * latePerCredit) <= 2147483647; */
/*@ assert rte_18: signed_overflow: -2147483648 <= tuitionBalance + (int)((int)(enrollmentCredits * costPerCredit) + (int)(enrollmentCredits * latePerCredit)); */
/*@ assert rte_19: signed_overflow: tuitionBalance + (int)((int)(enrollmentCredits * costPerCredit) + (int)(enrollmentCredits * latePerCredit)) <= 2147483647; */
	tuitionBalance += enrollmentCredits * costPerCredit + enrollmentCredits * latePerCredit;
	return;
}

/*@
	requires lateRegistration == 0 || lateRegistration == 1;
	requires tuitionBalance <= 0;
	requires 0 <= enrollmentCredits <= maxSemesterCredits;
	requires -2147483648 <= tuitionBalance + enrollmentCredits * costPerCredit <= 2147483647;
	requires tuitionBalance >= -2147483648 + enrollmentCredits * (costPerCredit + costPerCredit / 100 * 6);
	assigns tuitionBalance, enrollmentCredits, passedCredits, id, lateRegistration;
*/
void enroll(void)
{
/*@ assert rte_24: bool_value: lateRegistration == 0 || lateRegistration == 1; */
	if (lateRegistration) lateEnrollment(); else regularEnrollment();
	return;
}

/*@
	requires payment > 0;
	requires -2147483648 <= tuitionBalance - payment;
	requires tuitionBalance - payment <= 2147483647;
	assigns tuitionBalance;
	ensures (payment > 0) ==> (tuitionBalance == \old(tuitionBalance) - payment);
	ensures (payment <= 0) ==> (\false);
*/
void payTuitionWithDebitCard(int payment)
{
	_Bool tmp;
	tmp = validPayment(payment);
	if (! tmp) exit(-1);
/*@ assert rte_25: signed_overflow: -2147483648 <= tuitionBalance - payment; */
/*@ assert rte_26: signed_overflow: tuitionBalance - payment <= 2147483647; */
	tuitionBalance -= payment;
	return;
}

/*@
	requires payment > 0;
	requires (payment / 100) * 2 >= -2147483648;
	requires (payment / 100) * 2 <= 2147483647;
	requires payment - (payment / 100) * 2 >= -2147483648;
	requires payment - (payment / 100) * 2 <= 2147483647;
	requires tuitionBalance - (payment - (payment / 100) * 2) >= -2147483648;
	requires tuitionBalance - (payment - (payment / 100) * 2) <= 2147483647;
	assigns tuitionBalance;
	ensures (\old(payment) - (\old(payment) / 100) * 2 > 0) ==>(tuitionBalance == \old(tuitionBalance) - (\old(payment) - (\old(payment) / 100) * 2));
	ensures (\old(payment) - (\old(payment) / 100) * 2 <= 0) ==>(tuitionBalance == \old(tuitionBalance));
*/
void payTuitionWithCreditCard(int payment)
{
	_Bool tmp;
/*@ assert rte_27: signed_overflow: -2147483648 <= (int)(payment / 100) * 2; */
/*@ assert rte_28: signed_overflow: (int)(payment / 100) * 2 <= 2147483647; */
	int penalty = (payment / 100) * 2;
/*@ assert rte_29: signed_overflow: -2147483648 <= payment - penalty; */
/*@ assert rte_30: signed_overflow: payment - penalty <= 2147483647; */
	int actualPayment = payment - penalty;
	tmp = validPayment(actualPayment);
	if (! tmp) exit(-1);
/*@ assert rte_31: signed_overflow: -2147483648 <= tuitionBalance - actualPayment; */
/*@ assert rte_32: signed_overflow: tuitionBalance - actualPayment <= 2147483647; */
	tuitionBalance -= actualPayment;
	return;
}

/*@
	requires payment > 0;
	requires -2147483648 <= tuitionBalance - payment;
	requires tuitionBalance - payment <= 2147483647;
	requires (payment / 100) * 2 >= -2147483648;
	requires (payment / 100) * 2 <= 2147483647;
	requires payment - (payment / 100) * 2 >= -2147483648;
	requires payment - (payment / 100) * 2 <= 2147483647;
	requires tuitionBalance - (payment - (payment / 100) * 2) >= -2147483648;
	requires tuitionBalance - (payment - (payment / 100) * 2) <= 2147483647;
	assigns tuitionBalance;
	ensures (debit != 0) ==> (tuitionBalance == \old(tuitionBalance) - payment);
	ensures (debit == 0) && (\old(payment) - (\old(payment) / 100) * 2 > 0) ==> (tuitionBalance == \old(tuitionBalance) - (\old(payment) - (\old(payment) / 100) * 2));
	ensures (debit == 0) && (\old(payment) - (\old(payment) / 100) * 2 <= 0) ==> (tuitionBalance == \old(tuitionBalance));
*/
void pay(int payment, _Bool debit)
{
	if (debit) payTuitionWithDebitCard(payment);
	else payTuitionWithCreditCard(payment);
	return;
}

/*@
	requires costPerCredit == 200;
	requires maxSemesterCredits == 20;
	requires totalCredits == 120;
	requires initBalance >= 0;
	requires initBalance <= maxSemesterCredits * costPerCredit + maxSemesterCredits * ((costPerCredit / 100) * 6);
	requires pCredits >= 0;
	requires pCredits <= totalCredits;
	requires sCredits >= 0;
	requires sCredits <= maxSemesterCredits;
	requires isLate == (_Bool)0 || isLate == (_Bool)1;
	requires debit == (_Bool)0 || debit == (_Bool)1;
	requires option == (_Bool)0 || option == (_Bool)1;
	requires option ==> initBalance <= 0;
	requires option ==> lateRegistration == 0 || lateRegistration == 1;
	requires option ==> -2147483648 <= initBalance + sCredits * costPerCredit;
	requires option ==> initBalance + sCredits * costPerCredit <= 2147483647;
	requires option ==> initBalance >= -2147483648 + sCredits * (costPerCredit + costPerCredit / 100 * 6);
	requires !option ==> payment > 0;
	requires !option ==> -2147483648 <= initBalance - payment;
	requires !option ==> initBalance - payment <= 2147483647;
	requires !option ==> (payment / 100) * 2 >= -2147483648;
	requires !option ==> (payment / 100) * 2 <= 2147483647;
	requires !option ==> payment - (payment / 100) * 2 >= -2147483648;
	requires !option ==> payment - (payment / 100) * 2 <= 2147483647;
	requires !option ==> initBalance - (payment - (payment / 100) * 2) >= -2147483648;
	requires !option ==> initBalance - (payment - (payment / 100) * 2) <= 2147483647;
	assigns tuitionBalance, passedCredits, enrollmentCredits, lateRegistration, id;
	ensures (option == (_Bool)0) && (debit != (_Bool)0) ==>tuitionBalance == initBalance - payment;
	ensures (option == (_Bool)0) && (debit == (_Bool)0) && (payment - (payment / 100) * 2 > 0) ==>tuitionBalance == initBalance - (payment - (payment / 100) * 2);
	ensures (option == (_Bool)0) && (debit == (_Bool)0) && (payment - (payment / 100) * 2 <= 0) ==>tuitionBalance == initBalance;
	ensures (option == (_Bool)0) ==>passedCredits == \old(pCredits);
	ensures (option == (_Bool)0) ==>enrollmentCredits == \old(sCredits);
	ensures (option == (_Bool)0) ==>lateRegistration == \old(isLate);
	ensures (option == (_Bool)0) && (\old(debit) != (_Bool)0) ==>tuitionBalance == \old(initBalance) - \old(payment);
	ensures (option == (_Bool)0) && (\old(debit) == (_Bool)0)&& (\old(payment) - (\old(payment) / 100) * 2 > 0) ==>tuitionBalance == \old(initBalance) - (\old(payment) - (\old(payment) / 100) * 2);
	ensures (option == (_Bool)0) && (\old(debit) == (_Bool)0)&& (\old(payment) - (\old(payment) / 100) * 2 <= 0) ==>tuitionBalance == \old(initBalance);
	ensures (option == (_Bool)0) && (\old(debit) == (_Bool)0)&& (\old(payment) - (\old(payment) / 100) * 2 > 0) ==>tuitionBalance == \old(initBalance)- (\old(payment) - (\old(payment) / 100) * 2);
*/
void enrollmentProcess(int pCredits, int sCredits, int payment,
int initBalance, _Bool isLate, _Bool debit,
_Bool option)
{
/*@ assert rte_33: signed_overflow: -2147483648 <= maxSemesterCredits * costPerCredit; */
/*@ assert rte_34: signed_overflow: maxSemesterCredits * costPerCredit <= 2147483647; */
/*@ assert rte_35: signed_overflow: -2147483648 <= (int)(costPerCredit / 100) * 6; */
/*@ assert rte_36: signed_overflow: (int)(costPerCredit / 100) * 6 <= 2147483647; */
/*@ assert rte_37: signed_overflow: -2147483648 <= maxSemesterCredits * (int)((int)(costPerCredit / 100) * 6); */
/*@ assert rte_38: signed_overflow: maxSemesterCredits * (int)((int)(costPerCredit / 100) * 6) <= 2147483647; */
/*@ assert rte_39: signed_overflow: -2147483648 <= (int)(maxSemesterCredits * costPerCredit) + (int)(maxSemesterCredits * (int)((int)(costPerCredit / 100) * 6)); */
/*@ assert rte_40: signed_overflow: (int)(maxSemesterCredits * costPerCredit) + (int)(maxSemesterCredits * (int)((int)(costPerCredit / 100) * 6)) <= 2147483647; */
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
/*@ assert tuitionBalance == 3180; */ ;
	__retres = 0;
	return __retres;
}


