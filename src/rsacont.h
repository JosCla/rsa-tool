#ifndef RSACONT_H
#define RSACONT_H
// rsacont.h
// Contains the RSAContainer class, which manages an RSA object and which actions it performs.

// C++ Standard Library
#include <string>
#include <vector>

// Our Files
#include "rsaobj.h"

struct OutputType {
	std::string path = "";
	std::string attrs = "";
};

class RSAContainer {
public:
	RSAContainer(const RSA &rsa = RSA());

	void parseArgs(const std::vector<std::string> &args);
	int inputFromFile(const std::string &filepath);

	int evaluate();
	void doOutput() const;

	RSA getRSAObj() const;
	void setRSAObj(const RSA &rsa);
	int getMode() const;
	void setMode(const int mode);
	std::vector<OutputType> getOutVec() const;
	void setOutVec(const std::vector<OutputType> &outVec);
	void addToOutVec(const OutputType &out);

private:
	RSA _rsa;

	int _mode;
	std::vector<OutputType> _outVec;
};

#endif // RSACONT_H
