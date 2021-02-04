// rsaobj.cpp
// Defines the class defined in rsaobj.h

// C++ Standard Library
#include <iostream>
using std::cout; using std::endl; using std::ostream;
#include <string>
using std::string;
#include <vector>
using std::vector;

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

	// Performing any needed basic regeneration.
	basicRegen();

	// Trying common attacks
	if (_e != 0 && _n != 0 && (_p == 0 || _q == 0 || _totN == 0 || _d == 0)) {
		wienersAttack();
	}

	// Randomly regenerating any remaining properties.
	if (_p == 0) {
		_p = genPrime(200);
		randGen += 'p';
	}
	if (_q == 0) {
		_q = genPrime(200);
		randGen += 'q';
	}
	if (_n == 0) {
		_n = _p * _q;
		randGen += 'n';
	}
	if (_totN == 0) {
		_totN = (_p - 1) * (_q - 1);
		randGen += 't';
	}
	if (_e == 0) {
		_e = genPrime(5);
		randGen += 'e';
	}
	if (_d == 0) {
		_d = modMultInv(_e, _totN);
		while (_d < 0) {_d += _totN;}
		randGen += 'd';
	}

	return randGen;
}

// Performs basic regeneration on the key components in RSA.
void RSA::basicRegen() {
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
		} else if (_n != 0 && _totN != 0) {
			mpz_class pPlusQ = _n - _totN + 1;
			mpz_class discrim = (pPlusQ * pPlusQ) - (4 * _n);
			if (discrim > 0) {
				mpz_class middle = pPlusQ / 2;
				mpz_class diff = sqrt(discrim) / 2;
				_p = middle - diff;
				_q = middle + diff;
			}
		}
	}

	// Regenerating n and totN
	if (_n == 0) {
		if (_p != 0 && _q != 0) {
			_n = _p * _q;
		}
	}
	if (_totN == 0) {
		if (_p != 0 && _q != 0) {
			_totN = (_p - 1) * (_q - 1);
		}
	}

	// Regenerating e
	if (_e == 0) {
		if (_d != 0 && _totN != 0) {
			_e = modMultInv(_d, _totN);
			while (_e < 0) {_e += _totN;}
		}
	}

	// Regenerating d (same process as e)
	if (_d == 0) {
		if (_e != 0 && _totN != 0) {
			_d = modMultInv(_e, _totN);
			while (_d < 0) {_d += _totN;}
		}
	}
}

// Wiener's Attack
// Guide: https://sagi.io/crypto-classics-wieners-rsa-attack/
// Works when private exponent d is smaller than about n^(1/4}
// Potentially recovers all key components from e and n
// Returns true if it succeeded, false if it failed
bool RSA::wienersAttack() {
	bool success = false;

	// Generating the continued fraction of e / n
	vector<mpz_class> enFrac;
	continuedFrac(_e, _n, enFrac);

	// Iterating through the convergents
	for (unsigned int ind = 0; ind < enFrac.size(); ++ind) {
		// Getting current convergent and putting it into K and D guesses
		mpz_class currK, currD;
		getConvergent(enFrac, ind, currK, currD);
		if (currK == 0) {continue;}

		// Getting totN from the current K and D
		mpz_class currTotN = (_e * currD - 1) / currK;

		// Getting p and q from this totN and the given n
		mpz_class currP(0), currQ(0);
		mpz_class pPlusQ = _n - currTotN + 1;
		mpz_class discrim = (pPlusQ * pPlusQ) - (4 * _n);
		if (discrim > 0) {
			mpz_class middle = pPlusQ / 2;
			mpz_class diff = sqrt(discrim) / 2;
			currP = middle - diff;
			currQ = middle + diff;
		}

		// If p * q = n, we've found a match!
		if (currP * currQ == _n) {
			_p = currP;
			_q = currQ;
			_totN = currTotN;
			_d = currD;
			
			success = true;
			break;
		}

		// Also, if d gets too large, exit
		if ((currD * currD * currD * currD) > _n) {
			break;
		}
	}

	return success;
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
