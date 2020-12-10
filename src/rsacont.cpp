// rsacont.cpp
// Implements rsacont.h.

// C++ Standard Library
#include <iostream>
using std::cout; using std::endl;
#include <fstream>
using std::ifstream; using std::ofstream;
#include <string>
using std::string;
#include <vector>
using std::vector;

// Libraries
#include <gmpxx.h>

// Our Files
#include "rsacont.h"
#include "rsaobj.h"

RSAContainer::RSAContainer(const RSA &rsa) : _rsa(rsa) {
	_mode = -1;
}

void RSAContainer::parseArgs(const vector<string> &args) {
	for (string arg : args) {
		// Making sure that the argument starts with a hyphen
		if (arg.at(0) != '-') {continue;}

		// Parsing the argument
		switch (arg.at(1)) {
			case 'g':
				// Set mode: Generate Key
				_mode = 0;
				break;
			case 'e':
				// Set mode: Encrypt
				_mode = 1;
				break;
			case 'd':
				// Set mode: Decrypt
				_mode = 2;
				break;
		
			case 'i':
				// Input
				if (arg.at(2) == '=') {
					// (from file)
					inputFromFile(arg.substr(3));
				} else {
					// (from argument)
					if (arg.at(2) == 's') {
						_rsa.setProp('m', arg.substr(4));
					} else {
						_rsa.setProp(arg.at(2), mpz_class(arg.substr(4)));
					}
				}
				break;

			case 'o':
				// Output
				int equalsInd = arg.find('=');

				OutputType out;
				if (equalsInd != -1) {
					// (to file)
					out.attrs = arg.substr(2, equalsInd - 2);
					out.path = arg.substr(equalsInd + 1);
				} else {
					// (to std::cout)
					out.attrs = arg.substr(2);
				}

				addToOutVec(out);
				break;
		}
	}
}

// Inputs properties from a file into the RSA object
int RSAContainer::inputFromFile(const string &filepath) {
	// Opening the file
	ifstream fileIn;
	fileIn.open(filepath);
	if (fileIn.fail()) {
		return -1;
	}

	// Reading the file's contents
	while(!fileIn.eof()) {
		string currLine;
		getline(fileIn, currLine);
		if (fileIn.eof()) {
			break;
		}

		char prop = currLine.at(0);
		if (prop == 's') {
			_rsa.setProp('m', currLine.substr(3));
		} else {
			_rsa.setProp(prop, mpz_class(currLine.substr(3)));
		}
	}

	// Closing the file
	fileIn.close();

	return 0;
}

// Uses the given mode and inputs to perform expected calculations
int RSAContainer::evaluate() {
	switch (_mode) {
		case 0:
			// Generate key
			{
			string randGen = _rsa.genKeyFrom();
			cout << "Generated randomly: " << randGen << endl;
			doOutput();
			}
			break;
		
		case 1:
			// Encrypt
			if (_rsa.encrypt()) {
				if (_outVec.empty()) {
					OutputType defaultOut = {"", "c"};
					addToOutVec(defaultOut);
				}
				doOutput();
			} else {
				return -2;
			}
			break;

		case 2:
			// Decrypt
			if (_rsa.decrypt()) {
				if (_outVec.empty()) {
					OutputType defaultOut = {"", "m"};
					addToOutVec(defaultOut);
				}
				doOutput();
			} else {
				return -2;
			}
			break;

		default:
			// Invalid mode
			return -1;
			break;
	}

	return 0;
}

void RSAContainer::doOutput() const {
	for (OutputType out : _outVec) {
		if (out.path == "") {
			// Printing to std::cout
			_rsa.printProps(out.attrs);
		} else {
			// Printing to a file
			ofstream fileOut;
			fileOut.open(out.path);
			if (fileOut.fail()) {
				continue;
			}

			_rsa.printProps(out.attrs, fileOut);

			fileOut.close();
		}
	}
}

// Getter/setter for RSA object
RSA RSAContainer::getRSAObj() const {
	return _rsa;
}
void RSAContainer::setRSAObj(const RSA &rsa) {
	_rsa = rsa;
}

// Getter/setter for mode
int RSAContainer::getMode() const {
	return _mode;
}
void RSAContainer::setMode(const int mode) {
	_mode = mode;
}

// Getters/setters for outputs
vector<OutputType> RSAContainer::getOutVec() const {
	return _outVec;
}
void RSAContainer::setOutVec(const vector<OutputType> &outVec) {
	_outVec = outVec;
}
void RSAContainer::addToOutVec(const OutputType &out) {
	_outVec.push_back(out);
}
