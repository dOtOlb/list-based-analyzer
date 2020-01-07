#include "ChainAnalyzer.h"
#include "GeoPerfAnalyzer.h"
#include <iostream>
#include <fstream>
#include "utilities.h"
#include <memory>

using namespace std;


/**
 * Constructor
 *
 * @param
 */
ChainAnalyzer::ChainAnalyzer() : chain(), line_count(0) {
	// the default list of analyzers
	chain.push_back(static_cast<std::shared_ptr<Analyzer>>(new GeoPerfAnalyzer()));
}

/**
 * Destructor
 *
 * @param
 */
ChainAnalyzer::~ChainAnalyzer() =default;

/**
 * The initializer to setup the basic data structure according to the command line arguments
 *
 * @param argc
 * @param argv
 */
void ChainAnalyzer::initialize(int argc, char** argv) {
	// initializes the network info structure
	string ip_to_region_mapfile, region_to_geo_mapfile;

	// first, read the related command line arguments
	for (int i=argc-1; i>0; i--) {
		// option -i: ip to region mapping file
		if (strcmp(argv[argc-i],"-i")==0) {
			ip_to_region_mapfile = argv[argc-i+1];
		}
		// option -g: region to geo mapping file
		if (strcmp(argv[argc-i],"-g")==0) {
			region_to_geo_mapfile = argv[argc-i+1];
		}
	}

	// second, open/read the files and populate the tables
	this->network_info.initIpToRegion(ip_to_region_mapfile);
	this->network_info.initRegionToGeo(region_to_geo_mapfile);

    // loop through the chain of stream parsers
    for (vector<std::shared_ptr<Analyzer>>::iterator itr = chain.begin(); itr != chain.end(); itr++) {
		(*itr)->initialize(this->network_info);
		(*itr)->initialize(argc, argv);
	}
}

/**
 * The initializer to setup the network info data structure
 *
 * @param n the NetworkInfo object needed for getting the concerned network info
 */
void ChainAnalyzer::initialize(NetworkInfo n) {

}

/**
 * The data stream parser function to parse a string at a time
 *
 * @param
 */
int ChainAnalyzer::parseString(string s) {
	return this->parse(tokenize(s));
}

/**
 * The data stream parser function to parse a (tokenized) string at a time
 *
 * @param
 */
int ChainAnalyzer::parse(vector<string> ts) {
	int intermediate_result;

    // basic tracking
    this->line_count++;

    // loop through the chain of stream parsers
    for (vector<std::shared_ptr<Analyzer>>::iterator itr = chain.begin(); itr != chain.end(); itr++) {
//    	cout << "debug: " << this->line_count << "\n";
//    	cout << "debug 2: " << ts[1] << "\n";
    	if ((intermediate_result = (*itr)->parse(ts)) != ALL_GOOD) {
    		this->error_msg = "error in" + (*itr)->getName() + ": " + (*itr)->getError();
    		return intermediate_result;
    	};
    }
	return ALL_GOOD;
}

/**
 * Add analyzer into the chain
 *
 * @param
 */
void ChainAnalyzer::addAnalyzer() {
}

/**
 * Remove analyzer from the chain
 *
 * @param
 */
void ChainAnalyzer::removeAnalyzer() {
}

/**
 * Analyze the parsed data to generate result
 *
 * @param
 */
void ChainAnalyzer::analyze() {
}

/**
 * Publish result in a specified format to a specified output facility
 *
 * @param
 */
void ChainAnalyzer::publish() {
	cout << "line count: " << this->line_count << endl;

	// for each of the child analyzers
    for (vector<std::shared_ptr<Analyzer>>::iterator itr = chain.begin(); itr != chain.end(); itr++) {
    	(*itr)->publish();
	}
}

/**
 * Return the error message
 *
 * @param
 */
string ChainAnalyzer::getError() {
	return this->error_msg;
}
