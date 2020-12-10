#ifndef RSAFUNCS_H
#define RSAFUNCS_H
// rsafuncs.h
// Declares various functions used for RSA encryption

// C++ Standard Library
#include <string>

// Libraries
#include <gmpxx.h>

void genKey(mpz_class &e, mpz_class &d, mpz_class &n, const unsigned int pqLen = 200, const unsigned int eLen = 5);
mpz_class genPrime(const unsigned int numDigits);
mpz_class modMultInv(const mpz_class &num, const mpz_class &modulus);
mpz_class modExp(const mpz_class &num, const mpz_class &exp, const mpz_class &mod);
mpz_class stringToNum(const std::string &str);
std::string numToString(mpz_class num);

#endif // RSAFUNCS_H
