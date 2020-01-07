/**
 * @file
 * @author  dOtOlb <dOtOlb@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * The license under which the software will be distributed is TBD
 *
 * @section DESCRIPTION
 *
 * The main function for the software
 */

using namespace std;

#include <iostream>
#include "Analyzer.h"
#include "ChainAnalyzer.h"

// main method
int main(int argc, char *argv[]) {
	string s;
	ChainAnalyzer ca;	// this variable is key since we need to use chain analyzer as a container later

    // cout << "hello world" << endl;

		// initialize the chain analzyer first
    ca.initialize(argc, argv);

    // assuming all input from STDIN
    while (getline(cin, s)) {
    	if (ca.parseString(s) != Analyzer::ALL_GOOD) {
    	   cout << ca.getError() << endl;
    	   return -1;
    	};
    }

    // ca.analyze();
    cout << "analysis completed" << endl;
    ca.publish();

	return 0;
}
