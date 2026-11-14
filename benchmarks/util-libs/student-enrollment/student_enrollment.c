#include <stdlib.h>
#include <stdbool.h>


const int costPerCredit = 200;
const int totalCredits = 120;
const int maxSemesterCredits = 20;

int id = 0;
int passedCredits = 0;
int enrollmentCredits = 0;
int tuitionBalance = 0;
bool lateRegistration = false;



void init_StudentEnrollment(int newId) {
    id = newId;
    passedCredits = 0;
    enrollmentCredits = 0;
    tuitionBalance = 0;
}



void setTuitionBalance(int balance) {
    int maxTuition = maxSemesterCredits * costPerCredit + maxSemesterCredits * ((costPerCredit/100)*6);
    if (balance > maxTuition) {
        exit(-1);
    }
    tuitionBalance = balance;
}



void setPassedCredits(int credits) {
    if (credits < 0 || credits > totalCredits) {
        exit(-1);
    }
    passedCredits = credits;
}



void setEnrollmentCredits(int credits) {
    if (credits < 0 || credits > maxSemesterCredits) {
        exit(-1);
    }
    enrollmentCredits = credits;
}



void setLateRegistration(bool isLate) {
    lateRegistration = isLate;
}



int getTuition() { return tuitionBalance; }



int getEnrollmentCredits() { return enrollmentCredits; }



int getPassedCredits() { return passedCredits; }



bool getLateRegistrations() { return lateRegistration; }



int getId() { return id; }



bool registrationPermission() {
    return tuitionBalance <= 0;
}



bool validPayment(int payment) {
    return payment > 0;
}



void regularEnrollment() {
    if (!(tuitionBalance <= 0)) {
        exit(-1);
    }
    tuitionBalance += enrollmentCredits * costPerCredit;
}



void lateEnrollment() {
    if (!(tuitionBalance <= 0)) {
        exit(-1);
    }
    int latePerCredit = (costPerCredit/100)*6;
    tuitionBalance += enrollmentCredits * costPerCredit + enrollmentCredits * latePerCredit;
}



void enroll() {
    if (lateRegistration) {
        lateEnrollment();
    } else {
        regularEnrollment();
    }
}



void payTuitionWithDebitCard(int payment) {
    if (!validPayment(payment)) {
        exit(-1);
    }
    tuitionBalance -= payment;
}



void payTuitionWithCreditCard(int payment) {
    int penalty = (payment/100)*2;
    int actualPayment = payment - penalty;
    if (!validPayment(actualPayment)) {
        exit(-1);
    }
    tuitionBalance -= actualPayment;
}



void pay(int payment, bool debit) {
    if (debit) {
        payTuitionWithDebitCard(payment);
    } else {
        payTuitionWithCreditCard(payment);
    }
}


void enrollmentProcess(int pCredits, int sCredits, int payment, int initBalance, bool isLate, bool debit, bool option) {
    
    int maxTuition = maxSemesterCredits * costPerCredit + maxSemesterCredits * ((costPerCredit/100)*6);
    if (initBalance > maxTuition || pCredits < 0 || pCredits > totalCredits || sCredits < 0 || sCredits > maxSemesterCredits) {
        exit(-1);
    }

    
    setTuitionBalance(initBalance);
    setPassedCredits(pCredits);
    setEnrollmentCredits(sCredits);
    setLateRegistration(isLate);

    
    if (option) {
        enroll();
    } else {
        pay(payment, debit);
    }
}


int main() {
    int newId = 114514;
    init_StudentEnrollment(newId);   
    
    enrollmentProcess(30, 15, 0, 0, true, false, true);
    //@ assert tuitionBalance == 3180;

    return 0;
}