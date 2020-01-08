#include "GeoPerfAnalyzer.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

/**
 * Constructor (here we have to specify upfront some fixed data values like size buckets as well)
 *
 * @param
 */
GeoPerfAnalyzer::GeoPerfAnalyzer() : size_buckets {0, 100000, 1000000, 5000000, 20000000, 100000000, 1000000000},
									size_bucket_names {"100KB", "1MB", "5MB", "20MB", "100MB", "1GB", "1GB+"} {
}

/**
 * Destructor
 *
 * @param
 */
GeoPerfAnalyzer::~GeoPerfAnalyzer() =default;

/**
 * The initializer to setup the basic data structure according to the command line arguments
 *
 * @param argc
 * @param argv
 */
void GeoPerfAnalyzer::initialize(int argc, char** argv) {
	if (!this->network_info.isEmpty()) {
		// do nothing
	}
	else {
		// simply exit as the initialization is not propery supported yet
		cout << "analyzer initialization not supported yet!" << endl;
		exit(EXIT_FAILURE);
	}
}

/**
 * The initializer to setup the network info data structure
 *
 * @param n the network info parameter
 */
void GeoPerfAnalyzer::initialize(NetworkInfo n) {
	this->network_info = n;
}

/**
 * if it is an r line
 *
 * @param ts the tokenized string to be passed in
 */
bool GeoPerfAnalyzer::isrLine(vector<string> ts) {
	return (ts[1] == "r");
}

/**
 * if it is an f line
 *
 * @param ts the tokenized string to be passed in
 */
bool GeoPerfAnalyzer::isfLine(vector<string> ts) {
	return (ts[1] == "f");
}

/**
 * The data stream parser function to parse a (tokenized) string at a time
 *
 * @param ts the tokenized string to be passed in
 */
int GeoPerfAnalyzer::parse(vector<string> ts) {
	string region, client_region, client_geo;
	int obj_size;
	string ts_key_by_region, ts_key_by_geo;

	if (this->isrLine(ts)) {
		//cout << ts[0];

		// create Transaction
		Tx t(ts);

//		cout << fixed << setprecision(2) << t.timestamp << " " << t.req_id << " " << t.ghost_ip << "\n";

		// find the appropriate time series to add the transaction to
		region = this->network_info.getRegion(t.ghost_ip);
		client_region = this->network_info.getRegion(t.client_ip);
		client_geo = this->network_info.getGeo(client_region);

		obj_size = t.total_size;
//		cout << "obj size: " << computeSizeString(obj_size) << "\n";

		ts_key_by_region = this->makeKeyByRegion(region, client_region, obj_size);
		ts_key_by_geo = this->makeKeyByGeo(region, client_geo, obj_size);

		map<string, TimeSeries<Tx>>::iterator itr = result.find(ts_key_by_region);

		if (itr == result.end()) {
			TimeSeries<Tx> new_series(300);
			new_series.add(t.timestamp, t);

			result.insert(make_pair(ts_key_by_region, new_series));
			//result[ts_key_by_region] = TimeSeries<Tx>(300);
		}
		else
			itr->second.add(t.timestamp, t);

		itr = result.find(ts_key_by_geo);

		if (itr == result.end()) {
			TimeSeries<Tx> new_series(300);
			new_series.add(t.timestamp, t);

			result.insert(make_pair(ts_key_by_geo, new_series));
		}
		else
			itr->second.add(t.timestamp, t);
	}
	return ALL_GOOD;
}

/**
 * The computeSizeString function
 *
 * @param obj_size object size
 */
 string GeoPerfAnalyzer::computeSizeString(int obj_size) {
	int num_buckets = sizeof(size_buckets)/sizeof(*size_buckets);

	// iterate through until a proper size string is found
	for (int i = num_buckets - 1; i >= 0; i--) {
		if (obj_size >= size_buckets[i])
			return size_bucket_names[i];
	};

	return "";
}

/**
 * Make the key using region info
 *
 * @param region
 * @param client_region
 * @param obj_size
 */
string GeoPerfAnalyzer::makeKeyByRegion(string region, string client_region, int obj_size) {
	return "R_" + region + "_" + client_region + "_" + computeSizeString(obj_size);
}

/**
 * Make the key using geo info
 *
 * @param region
 * @param client_geo
 * @param obj_size
 */
string GeoPerfAnalyzer::makeKeyByGeo(string region, string client_geo, int obj_size) {
	return "G_" + region + "_" + client_geo + "_" + computeSizeString(obj_size);
}

/**
 * Analyze the parsed data to generate result
 * (this is still an empty function currently)
 *
 * @param
 */
void GeoPerfAnalyzer::analyze() {
}

/**
 * Publish result in a specified format to a specified output facility
 *
 * @param
 */
void GeoPerfAnalyzer::publish() {
	// assume being and end timing
	double begin_tp = 1483228800, end_tp = 1577836800;
	TxSetCalculator c(TxSetCalculator::AVERAGE_1);
	// plain old publishing of results
	for (map<string, TimeSeries<Tx>>::iterator itr = result.begin(); itr != result.end(); itr++) {
		TimeCurve tc(itr->second.begin(begin_tp), itr->second.end(end_tp), c);
		cout << "<<" << itr->first << ">>\n" << tc.toString();
		cout << "\n";
	}
}

/**
 * Return the error message
 *
 * @param
 */
string GeoPerfAnalyzer::getError() {
	return this->error_msg;
}
