#ifndef Tx_Included
#define Tx_Included

#include <vector>
#include <string>
#include "utilities.h"

using namespace std;

/**
 * The transaction (Tx) structure
 * TO DO: add in other checker functions
 * TO DO: add in other operator functions
 */
struct Tx {
	int content_size;
	int total_size;
	int transfer_time;
	int ssl_time;
	int req_end_time;
	int turnaround_time;
	string status_code;
	string error_code;
	string flag;
	double timestamp;
	string req_id;
	string arl;
	string byte_range;
	string ghost_ip;
	string client_ip;

	// extended fields
	float throughput;
	int total_time;

	/**
	 * Constructor from vector of string
	 *
	 * @param vs vector of string to be passed in
	 */
	Tx(vector<string> vs) {
		// read, convert and populate the fields
		content_size = TypedExtractor<int>::get(vs[8]);
		total_size = TypedExtractor<int>::get(vs[9]);
		transfer_time = TypedExtractor<int>::get(vs[6]);
		ssl_time = TypedExtractor<int>::get(vs[3]);
		req_end_time = TypedExtractor<int>::get(vs[4]);
		turnaround_time = TypedExtractor<int>::get(vs[5]);
		status_code = TypedExtractor<string>::get(vs[13]);
		error_code = TypedExtractor<string>::get(vs[14]);
		flag = TypedExtractor<string>::get(vs[15]);
		timestamp = TypedExtractor<double>::get(vs[2]);
		req_id = TypedExtractor<string>::get(vs[31]);
		arl = TypedExtractor<string>::get(vs[12]);
		byte_range = TypedExtractor<string>::get(vs[33]);
		ghost_ip = TypedExtractor<string>::get(vs[0]);
		client_ip = TypedExtractor<string>::get(vs[10]);

		total_time = ssl_time + req_end_time + turnaround_time + transfer_time;
		throughput = total_time == 0 ? -1 : total_size * 8 / total_time;
	}

	/**
	 * A checker function validating the throughput
	 */
	bool hasValidThroughput() {
		return (throughput != -1);
	}
};

/**
 * Operator ==
 *
 * @param lhs left hand side instance
 * @param rhs right hand side instance
 */
inline bool operator==(const Tx& lhs, const Tx& rhs) {
/*	if ((lhs.timestamp == rhs.timestamp) &&
	        (lhs.req_id == rhs.req_id) &&
	        (lhs.ghost_ip == rhs.ghost_ip) &&
	        (lhs.arl == rhs.arl)) {
		cout << "same\n";
		cout << lhs.timestamp << " " << rhs.timestamp << " "
		 << lhs.req_id << " " << rhs.req_id << " "
		  << lhs.ghost_ip << " " << rhs.ghost_ip << "\n";
	}*/
//	else
//		cout << "different\n";
	return ((lhs.timestamp == rhs.timestamp) &&
	        (lhs.req_id == rhs.req_id) &&
	        (lhs.ghost_ip == rhs.ghost_ip) &&
	        (lhs.arl == rhs.arl));
}

#endif
