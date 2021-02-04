// rsafuncs.cpp
// Implements functions declared in rsafuncs.h

// C++ Standard Library
#include <iostream>
using std::cout; using std::endl;
#include <string>
using std::string; using std::to_string;
#include <vector>
using std::vector;
#include <cstdlib>
using std::rand;

// Libraries
#include <gmpxx.h>

// Our files
#include "rsafuncs.h"

// Generates a key (public exp, private exp, modulus)
void genKey(mpz_class &e, mpz_class &d, mpz_class &n, const unsigned int pqLen, const unsigned int eLen) {
	// Generating large primes p and q
	mpz_class p = genPrime(pqLen);
	mpz_class q;
	do {
		q = genPrime(pqLen);
	} while (p == q);

	// Finding modulus n and its totient
	n = p * q;
	mpz_class totN = (p-1) * (q-1);

	// Generating public exponent e
	e = genPrime(eLen);

	// Finding private exponent d
	d = modMultInv(e, totN);
	while (d < 0) {
		d += totN; // (making d greater than zero)
	}
}

// Generates a random prime number given the number of digits it must contain
mpz_class genPrime(const unsigned int numDigits) {
	// Creating the string that will hold a large number, and making 
	// the first digit a random number from 1 to 9
	string numStr = to_string(rand() % 9 + 1);
	
	// Appending all remaining digits
	for (unsigned int i = 1; i < numDigits; ++i) {
		string currDigit = to_string(rand() % 10);
		numStr.append(currDigit);
	}

	// Making the number string into a number, and using finding the next prime 
	// that follows our large number
	mpz_class largeNum(numStr);
	mpz_class prime;
	mpz_nextprime(prime.get_mpz_t(), largeNum.get_mpz_t());

	// Returning our prime
	return prime;
}

// Computes the modular multiplicative inverse of a given number 
// within a given modulus, using the extended euclidean algorithm
// https://en.wikipedia.org/wiki/Modular_multiplicative_inverse
// https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm
mpz_class modMultInv(const mpz_class &num, const mpz_class &modulus) {
	// Setting up for extended euclidean algorithm
	mpz_class rOld(num); mpz_class r(modulus);
	mpz_class sOld(1); mpz_class s(0);
	mpz_class tOld(0); mpz_class t(1);

	// Finding greatest common divisor and bezout coefficients
	while (r != 0) {
		mpz_class divInt = rOld / r;
		mpz_class rNew = rOld - (divInt * r); rOld = r; r = rNew;
		mpz_class sNew = sOld - (divInt * s); sOld = s; s = sNew;
		mpz_class tNew = tOld - (divInt * t); tOld = t; t = tNew;
	}

	// Returning the first bezout coefficient, which can be proven to be
	// the modular multiplicative inverse
	return sOld;
}

// Computes num to the power of exp, in modulo mod
// Relies on the fact that (a^b) mod n = (a mod n)^b mod n
// and that (a*b) mod n = ((a mod n) * (b mod n)) mod n
mpz_class modExp(const mpz_class &num, const mpz_class &exp, const mpz_class &mod) {
	// Getting our exponent in binary form
	string expStr = exp.get_str(2);

	// Finding the number to the power of each bit of the exponent modded by mod,
	// and multiplying them all together
	mpz_class modProd(1);
	mpz_class currMod(num % mod);
	for (int i = expStr.length() - 1; i >= 0; --i) {
		// If the current bit is a one, multiply the current mod into the product
		if (expStr.at(i) == '1') {
			modProd *= currMod;
			modProd = modProd % mod;
		}

		// Changing the current mod for the next iteration
		currMod = (currMod * currMod) % mod;
	}

	// Returning the result
	return modProd;
}

// Converts a string from ASCII to decimal
mpz_class stringToNum(const std::string &str) {
	// Initializing return value
	mpz_class retNum(0);

	// For each letter, we convert it to decimal, add it on, and then
	// multiply by 256, so that this represents a hexadecimal character sequence
	for (unsigned int i = 0; i < str.length(); ++i) {
		retNum += (int)str.at(i);
		if (i != str.length() - 1) {retNum *= 256;}
	}

	// Returning our final value
	return retNum;
}

// Converts a number from decimal to ASCII
string numToString(mpz_class num) {
	// Initializing return value
	string retStr = "";

	// In order to find each character, we find the remainder when dividing
	// by 256, and append the character version to our return string
	while (num > 0) {
		mpz_class numMod = (num % 256);
		retStr = (char)(numMod.get_ui()) + retStr;
		num /= 256;
	}

	// Returning our final value
	return retStr;
}

// Finds a continued fraction given a numerator and denominator
void continuedFrac(const mpz_class &num, const mpz_class &den, vector<mpz_class> &res) {
	// Finding the current entry and the next numerator/denominator
	mpz_class rem = num % den;
	mpz_class intPart = (num - rem) / den;

	res.push_back(intPart);

	// If our remainder is not zero, we must continue reducing
	if (rem != mpz_class(0)) {
		continuedFrac(den, rem, res);
	}
}

// Finds a convergent through a given index
void getConvergent(const vector<mpz_class> &contFrac, const unsigned int ind, mpz_class &num, mpz_class &den) {
	// Starting with 0 / 1 for our convergent until we add terms
	num = 0;
	den = 1;

	// Going from the last term to the first
	// (collapsing the inner fractions before the outer ones)
	for (int currInd = ind; currInd >= 0; --currInd) {
		// Adding the current index
		num += contFrac.at(currInd) * den;

		// Then flipping the numerator and denominator if we need to move up a layer
		if (currInd > 0) {
			mpz_class swap = num;
			num = den;
			den = swap;
		}
	}
}
