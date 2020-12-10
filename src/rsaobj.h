#ifndef RSAOBJ_H
#define RSAOBJ_H
// rsaobj.h
// Declares a class that encapsulates RSA attributes and functionality

// C++ Standard Library
#include <iostream>
#include <string>

// Libraries
#include <gmpxx.h>

class RSA {
public:
	RSA();

	void genKey(const unsigned int pqLen = 200, const unsigned int eLen = 5);
	std::string genKeyFrom();
	void setProp(char prop, const mpz_class &val);
	void setProp(char prop, const std::string &str);
	void printProps(const std::string &props, std::ostream &out = std::cout) const;

	bool encrypt();
	bool decrypt();

private:
	mpz_class _m = 0;
	mpz_class _c = 0;

	mpz_class _e = 0;
	mpz_class _d = 0;
	mpz_class _n = 0;

	mpz_class _p = 0;
	mpz_class _q = 0;
	mpz_class _totN = 0;
};

#endif // RSAOBJ_H
