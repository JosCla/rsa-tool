// C++ Standard Library
#include <iostream>
using std::cout; using std::cerr; using std::endl;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <ctime>
using std::time;
#include <cstdlib>
using std::rand; using std::srand;

// Other libraries
#include <gmpxx.h>

// Our files
#include "rsacont.h"

int main(int argc, char *argv[]) {
	// Initializing randomization
	// TODO: get a better random number generation method
	srand(time(NULL));
	rand(); // (throwing away first random number to increase randomness)

	vector<string> args;
	for (int i = 0; i < argc; ++i) {
		args.push_back(argv[i]);
	}

	RSAContainer ourCont;
	ourCont.parseArgs(args);
	int evalStatus = ourCont.evaluate();
	if (evalStatus == -1) {
		cerr << "Invalid mode or mode not set." << endl;
		return -1;
	} else if (evalStatus == -2) {
		cerr << "Not enough information provided for mode selected." << endl;
		return -2;
	}

	return 0;
}
