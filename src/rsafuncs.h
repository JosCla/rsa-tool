#ifndef RSAFUNCS_H
#define RSAFUNCS_H
// rsafuncs.h
// Declares various functions used for RSA encryption

// C++ Standard Library
#include <string>
#include <vector>

// Libraries
#include <gmpxx.h>

// Our Files
#include "rsaobj.h"

void genKey(mpz_class &e, mpz_class &d, mpz_class &n, const unsigned int pqLen = 200, const unsigned int eLen = 5);
mpz_class genPrime(const unsigned int numDigits);
mpz_class modMultInv(const mpz_class &num, const mpz_class &modulus);
mpz_class modExp(const mpz_class &num, const mpz_class &exp, const mpz_class &mod);
mpz_class stringToNum(const std::string &str);
std::string numToString(mpz_class num);
void continuedFrac(const mpz_class &num, const mpz_class &den, std::vector<mpz_class> &res);
void getConvergent(const std::vector<mpz_class> &contFrac, const unsigned int ind, mpz_class &num, mpz_class &den);

#endif // RSAFUNCS_H
