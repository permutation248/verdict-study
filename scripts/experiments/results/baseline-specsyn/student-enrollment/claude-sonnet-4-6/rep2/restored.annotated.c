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
	requires costPerCredit == 200;
	requires maxSemesterCredits == 20;
	requires balance <= 2147483647;
	requires balance >= -2147483648;
	assigns tuitionBalance;
	ensures (balance > maxSemesterCredits * costPerCredit +maxSemesterCredits * ((costPerCredit / 100) * 6))==> \false;
	ensures (balance <= maxSemesterCredits * costPerCredit +maxSemesterCredits * ((costPerCredit / 100) * 6))==> tuitionBalance == balance;
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
	ensures (credits < 0 || credits > totalCredits) ==> \false;
	ensures (0 <= credits <= totalCredits) ==> passedCredits == credits;
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
	ensures (credits >= 0 && credits <= maxSemesterCredits) ==> (enrollmentCredits == credits);
	ensures (credits < 0 || credits > maxSemesterCredits) ==> (\false);
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
	requires \valid(&lateRegistration);
	requires isLate == 0 || isLate == 1;
	assigns lateRegistration;
	ensures (isLate == (_Bool)0) ==> (lateRegistration == (_Bool)0);
	ensures (isLate == (_Bool)1) ==> (lateRegistration == (_Bool)1);
	ensures lateRegistration == isLate;
*/
void setLateRegistration(_Bool isLate)
{
	lateRegistration = isLate;
	return;
}

/*@
	requires 0 <= tuitionBalance <= costPerCredit * totalCredits;
	assigns \nothing;
	ensures \result == tuitionBalance;
*/
int getTuition(void)
{
	return tuitionBalance;
}

/*@
	requires 0 <= enrollmentCredits <= maxSemesterCredits;
	assigns \nothing;
	ensures \result == enrollmentCredits;
	ensures 0 <= \result <= maxSemesterCredits;
*/
int getEnrollmentCredits(void)
{
	return enrollmentCredits;
}

/*@
	requires 0 <= passedCredits <= totalCredits;
	assigns \nothing;
	ensures \result == passedCredits;
	ensures (0 <= passedCredits <= totalCredits) ==> (0 <= \result <= totalCredits);
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
	ensures lateRegistration == 0 ==> \result == 0;
	ensures lateRegistration == 1 ==> \result == 1;
*/
_Bool getLateRegistrations(void)
{
/*@ assert rte_1: bool_value: lateRegistration == 0 || lateRegistration == 1; */
	return lateRegistration;
}

/*@
	requires id >= 0;
	assigns \nothing;
	ensures \result == id;
	ensures id >= 0 ==> \result >= 0;
	ensures id == 0 ==> \result == 0;
*/
int getId(void)
{
	return id;
}

/*@
	requires tuitionBalance >= -(costPerCredit * totalCredits);
	requires tuitionBalance <= (costPerCredit * totalCredits);
	assigns \nothing;
	ensures (tuitionBalance <= 0) ==> (\result == (_Bool)1);
	ensures (tuitionBalance > 0) ==> (\result == (_Bool)0);
*/
_Bool registrationPermission(void)
{
	_Bool __retres;
	__retres = (_Bool)(tuitionBalance <= 0);
	return __retres;
}

/*@
	requires payment >= 0;
	assigns \nothing;
	ensures (payment > 0) ==> (\result == \true);
	ensures (payment <= 0) ==> (\result == \false);
*/
_Bool validPayment(int payment)
{
	_Bool __retres;
	__retres = (_Bool)(payment > 0);
	return __retres;
}

/*@
	requires tuitionBalance + enrollmentCredits * costPerCredit >= -2147483648;
	requires 0 <= enrollmentCredits <= maxSemesterCredits;
	requires tuitionBalance <= 0;
	assigns tuitionBalance;
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
	requires 0 <= enrollmentCredits <= 2147483647 / (costPerCredit + (costPerCredit / 100) * 6);
	requires -2147483648 <= costPerCredit / 100;
	requires (costPerCredit / 100) * 6 <= 2147483647;
	requires -2147483648 <= enrollmentCredits * costPerCredit;
	requires enrollmentCredits * costPerCredit <= 2147483647;
	requires -2147483648 <= enrollmentCredits * (costPerCredit / 100) * 6;
	requires enrollmentCredits * (costPerCredit / 100) * 6 <= 2147483647;
	requires -2147483648 <= enrollmentCredits * costPerCredit + enrollmentCredits * ((costPerCredit / 100) * 6);
	requires enrollmentCredits * costPerCredit + enrollmentCredits * ((costPerCredit / 100) * 6) <= 2147483647;
	requires -2147483648 <= tuitionBalance + (enrollmentCredits * costPerCredit + enrollmentCredits * ((costPerCredit / 100) * 6));
	requires tuitionBalance + (enrollmentCredits * costPerCredit + enrollmentCredits * ((costPerCredit / 100) * 6)) <= 2147483647;
	assigns tuitionBalance;
	ensures (tuitionBalance <= 0) ==> (tuitionBalance == \old(tuitionBalance) + \old(enrollmentCredits) * costPerCredit + \old(enrollmentCredits) * (costPerCredit / 100) * 6);
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
	requires tuitionBalance + (enrollmentCredits * costPerCredit + enrollmentCredits * ((costPerCredit / 100) * 6)) <= 2147483647;
	requires -2147483648 <= tuitionBalance + (enrollmentCredits * costPerCredit + enrollmentCredits * ((costPerCredit / 100) * 6));
	requires enrollmentCredits * costPerCredit + enrollmentCredits * ((costPerCredit / 100) * 6) <= 2147483647;
	requires -2147483648 <= enrollmentCredits * costPerCredit + enrollmentCredits * ((costPerCredit / 100) * 6);
	requires enrollmentCredits * (costPerCredit / 100) * 6 <= 2147483647;
	requires -2147483648 <= enrollmentCredits * (costPerCredit / 100) * 6;
	requires enrollmentCredits * costPerCredit <= 2147483647;
	requires -2147483648 <= enrollmentCredits * costPerCredit;
	requires (costPerCredit / 100) * 6 <= 2147483647;
	requires -2147483648 <= costPerCredit / 100;
	requires tuitionBalance + enrollmentCredits * costPerCredit <= 2147483647;
	requires -2147483648 <= tuitionBalance + enrollmentCredits * costPerCredit;
	requires 0 <= enrollmentCredits <= maxSemesterCredits;
	requires tuitionBalance <= 0;
	requires lateRegistration == 0 || lateRegistration == 1;
	assigns tuitionBalance;
*/
void enroll(void)
{
/*@ assert rte_24: bool_value: lateRegistration == 0 || lateRegistration == 1; */
	if (lateRegistration) lateEnrollment(); else regularEnrollment();
	return;
}

/*@
	requires payment > 0;
	requires tuitionBalance - payment >= -2147483648;
	requires tuitionBalance - payment <= 2147483647;
	assigns tuitionBalance;
	ensures (payment <= 0) ==> (tuitionBalance == \old(tuitionBalance));
	ensures (payment > 0) ==>(-2147483648 <= \old(tuitionBalance) - payment &&\old(tuitionBalance) - payment <= 2147483647) ==>(tuitionBalance == \old(tuitionBalance) - payment);
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
	ensures (payment - (payment / 100) * 2 > 0) ==>(tuitionBalance == \old(tuitionBalance) - (payment - (payment / 100) * 2));
	ensures (payment - (payment / 100) * 2 <= 0) ==>(\false);
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
	requires tuitionBalance - payment >= -2147483648;
	requires tuitionBalance - payment <= 2147483647;
	requires (payment / 100) * 2 >= -2147483648;
	requires (payment / 100) * 2 <= 2147483647;
	requires payment - (payment / 100) * 2 >= -2147483648;
	requires payment - (payment / 100) * 2 <= 2147483647;
	requires tuitionBalance - (payment - (payment / 100) * 2) >= -2147483648;
	requires tuitionBalance - (payment - (payment / 100) * 2) <= 2147483647;
	assigns tuitionBalance;
	ensures (debit == \true) ==>(tuitionBalance == \old(tuitionBalance) - payment);
	ensures (debit == \false) ==>(tuitionBalance == \old(tuitionBalance) - (payment - (payment / 100) * 2));
*/
void pay(int payment, _Bool debit)
{
	if (debit) payTuitionWithDebitCard(payment);
	else payTuitionWithCreditCard(payment);
	return;
}

/*@
	requires !option ==> initBalance - (payment - (payment / 100) * 2) <= 2147483647;
	requires !option ==> initBalance - (payment - (payment / 100) * 2) >= -2147483648;
	requires !option ==> payment - (payment / 100) * 2 <= 2147483647;
	requires !option ==> payment - (payment / 100) * 2 >= -2147483648;
	requires !option ==> (payment / 100) * 2 <= 2147483647;
	requires !option ==> (payment / 100) * 2 >= -2147483648;
	requires !option ==> initBalance - payment <= 2147483647;
	requires !option ==> initBalance - payment >= -2147483648;
	requires !option ==> payment > 0;
	requires option ==> initBalance + (sCredits * costPerCredit + sCredits * ((costPerCredit / 100) * 6)) <= 2147483647;
	requires option ==> -2147483648 <= initBalance + (sCredits * costPerCredit + sCredits * ((costPerCredit / 100) * 6));
	requires option ==> initBalance + sCredits * costPerCredit <= 2147483647;
	requires option ==> -2147483648 <= initBalance + sCredits * costPerCredit;
	requires option ==> sCredits * costPerCredit + sCredits * ((costPerCredit / 100) * 6) <= 2147483647;
	requires option ==> -2147483648 <= sCredits * costPerCredit + sCredits * ((costPerCredit / 100) * 6);
	requires option ==> sCredits * (costPerCredit / 100) * 6 <= 2147483647;
	requires option ==> -2147483648 <= sCredits * (costPerCredit / 100) * 6;
	requires option ==> sCredits * costPerCredit <= 2147483647;
	requires option ==> -2147483648 <= sCredits * costPerCredit;
	requires option ==> (costPerCredit / 100) * 6 <= 2147483647;
	requires option ==> -2147483648 <= costPerCredit / 100;
	requires option ==> 0 <= sCredits <= maxSemesterCredits;
	requires option ==> initBalance <= 0;
	requires isLate == 0 || isLate == 1;
	requires sCredits <= maxSemesterCredits;
	requires sCredits >= 0;
	requires pCredits <= totalCredits;
	requires pCredits >= 0;
	requires initBalance <= maxSemesterCredits * costPerCredit+ maxSemesterCredits * ((costPerCredit / 100) * 6);
	requires initBalance <= 2147483647;
	requires initBalance >= -2147483648;
	requires \valid(&lateRegistration);
	requires totalCredits == 120;
	requires maxSemesterCredits == 20;
	requires costPerCredit == 200;
	assigns tuitionBalance, passedCredits, enrollmentCredits, lateRegistration;
	ensures passedCredits == pCredits;
	ensures enrollmentCredits == sCredits;
	ensures lateRegistration == isLate;
	ensures (!option && debit) ==>tuitionBalance == initBalance - payment;
	ensures (!option && !debit) ==>tuitionBalance == initBalance - (payment - (payment / 100) * 2);
	ensures (option == (_Bool)0 && debit == (_Bool)1) ==>tuitionBalance == initBalance - payment;
	ensures (option == (_Bool)0 && debit == (_Bool)0) ==>tuitionBalance == initBalance - (payment - (payment / 100) * 2);
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


