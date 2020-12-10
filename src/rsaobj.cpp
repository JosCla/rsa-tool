// rsaobj.cpp
// Defines the class defined in rsaobj.h

// C++ Standard Library
#include <iostream>
using std::cout; using std::endl; using std::ostream;
#include <string>
using std::string;

// Libraries
#include <gmpxx.h>

// Our files
#include "rsaobj.h"
#include "rsafuncs.h"

// Constructor. Does nothing by default
RSA::RSA() {}

// Generates a key given the length of primes p and q, and the length of e
void RSA::genKey(const unsigned int pqLen, const unsigned int eLen) {
	// Generating p and q
	_p = genPrime(pqLen);
	do {
		_q = genPrime(pqLen);
	} while (_p == _q);

	// Finding modulus n and its totient
	_n = _p * _q;
	_totN = (_p-1) * (_q-1);

	// Generating public exponent e
	_e = genPrime(eLen);

	// Finding private exponent d
	_d = modMultInv(_e, _totN);
	while (_d < 0) {
		_d += _totN; // (making sure d is greater than zero)
	}
}

// Generates a key from existing properties
// Returns a string of each property that had to be generated randomly
string RSA::genKeyFrom() {
	// String of each property that had to be generated randomly
	string randGen = "";

	// Regenerating large primes
	if (_p == 0 || _q == 0) {
		if (_n != 0 && _q != 0) {
			_p = _n / _q;
		} else if (_n != 0 && _p != 0) {
			_q = _n / _p;
		} else if (_totN != 0 && _q != 0) {
			_p = (_totN / (_q - 1)) + 1;
		} else if (_totN != 0 && _p != 0) {
			_q = (_totN / (_p - 1)) + 1;
		}
	}
	if (_p == 0) {
		_p = genPrime(200);
		randGen += 'p';
	}
	if (_q == 0) {
		_q = genPrime(200);
		randGen += 'q';
	}

	// Regenerating n and totN
	if (_n == 0) {
		_n = _p * _q;
		if (randGen.size() > 0) { // if p or q were generated
			randGen += 'n';
		}
	}
	if (_totN == 0) {
		_totN = (_p - 1) * (_q - 1);
		if (randGen.size() > 0) { // if p or q were generated
			randGen += 't';
		}
	}

	// Regenerating e
	if (_e == 0) {
		if (_d != 0) {
			_e = modMultInv(_d, _totN);
			while (_e < 0) {_e += _totN;}
			if (randGen.find('t') < randGen.length()) {
				randGen += 'e';
			}
		} else {
			_e = genPrime(5);
			randGen += 'e';
		}
	}

	// Regenerating d (same process as e)
	if (_d == 0) {
		_d = modMultInv(_e, _totN);
		while (_d < 0) {_d += _totN;}
		if (randGen.find('t') < randGen.length() || randGen.find('e') < randGen.length()) {
			randGen += 'd';
		}
	}

	return randGen;
}

// Sets a given property
void RSA::setProp(char prop, const mpz_class &val) {
	// Making sure value is positive
	if (val <= 0) {
		return;
	}

	// Setting the property
	switch (prop) {
		case 'm':
			_m = val;
			break;
		case 'c':
			_c = val;
			break;

		case 'e':
			_e = val;
			break;
		case 'd':
			_d = val;
			break;
		case 'n':
			_n = val;
			break;

		case 'p':
			_p = val;
			break;
		case 'q':
			_q = val;
			break;
		case 't':
			_totN = val;
			break;

		default:
			break;
	}
}

void RSA::setProp(char prop, const string &str) {
	mpz_class val = stringToNum(str);
	setProp(prop, val);
}

// Prints various properties of the RSA key
void RSA::printProps(const std::string &props, std::ostream &out) const {
	for (unsigned int i = 0; i < props.length(); ++i) {
		char currProp = props.at(i);
		switch (currProp) {
			case 'm':
				out << "m: " << _m << endl;
				break;
			case 'c':
				out << "c: " << _c << endl;
				break;
			case 's':
				out << "s: " << numToString(_m) << endl;
				break;

			case 'e':
				out << "e: " << _e << endl;
				break;
			case 'd':
				out << "d: " << _d << endl;
				break;
			case 'n':
				out << "n: " << _n << endl;
				break;

			case 'p':
				out << "p: " << _p << endl;
				break;
			case 'q':
				out << "q: " << _q << endl;
				break;
			case 't':
				out << "t: " << _totN << endl;
				break;

			default:
				break;
		}
	}
}

// Functions for encrypting and decrypting messages
bool RSA::encrypt() {
	if (_e > 0 && _n > 0 && _m > 0) {
		_c = modExp(_m, _e, _n);
		return true;
	} else {
		return false;
	}
}
bool RSA::decrypt() {
	if (_d > 0 && _n > 0 && _c > 0) {
		_m = modExp(_c, _d, _n);
		return true;
	} else {
		return false;
	}
}
