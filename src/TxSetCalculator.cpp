#include <math.h>
#include "TxSetCalculator.h"

using namespace std;

/**
 * Constructor
 *
 * @param
 */
TxSetCalculator::TxSetCalculator(TxSetAction a) {
	this->action = a;
}

/**
 * The operator function
 *
 * @param ts the time slot struct to be passed in
 */
float TxSetCalculator::operator() (TimeSeries<Tx>::timeslot ts) {
	switch (this->action) {
		case AVERAGE_1:		return this->findAverage1(ts.datapoints);
							break;
		case AVERAGE_2:		return this->findAverage2(ts.datapoints);
							break;
		case MEDIAN:		return this->findMedian(ts.datapoints);
							break;
		case PERCENTILE_95:	return this->find95Percentile(ts.datapoints);
							break;
	}
}

float TxSetCalculator::findAverage1(multimap<double, Tx>* d) {
	float avg_tpt = 0;
	int cnt = 0;

//	int debug_cnt = 0;

	if (d == NULL)
		return -1;
	for (multimap<double, Tx>::iterator itr = d->begin(); itr != d->end(); itr++) {
		if ((itr->second).hasValidThroughput()) {
			avg_tpt += (itr->second).throughput;
			cnt++;
		}

//		debug_cnt++;

	}
//	cout << "calc: " << cnt << " " << debug_cnt << "\n";
	if (cnt == 0)
		return -1;
	else
		return avg_tpt / cnt;
}

float TxSetCalculator::findAverage2(multimap<double, Tx>* d) {
	int total_size = 0;
	int total_time = 0;
	int cnt = 0;

	if (d == NULL)
		return -1;
	for (multimap<double, Tx>::iterator itr = d->begin(); itr != d->end(); itr++) {
		total_size += (itr->second).total_size;
		total_time += (itr->second).total_time;
		cnt++;
	}
	if ((cnt == 0) || (total_time == 0))
		return -1;
	else
		return static_cast<float>(total_size) * 8 / total_time;
}

void TxSetCalculator::sortByThroughput(multimap<double, Tx>* d, vector<float>& vf) {
	for (multimap<double, Tx>::iterator itr = d->begin(); itr != d->end(); itr++) {
		if ((itr->second).hasValidThroughput())
			vf.push_back((itr->second).throughput);
	}
	sort(vf.begin(), vf.end());
}

float TxSetCalculator::findMedian(multimap<double, Tx>* d) {
	vector<float> vf;
	if (d == NULL)
		return -1;
	sortByThroughput(d, vf);
	if (vf.size() == 0)
		return -1;
	else if (vf.size() % 2)
		return vf[vf.size() / 2];
	else
		return (vf[vf.size() / 2] + vf[vf.size() / 2 - 1]) / 2;
}

float TxSetCalculator::find95Percentile(multimap<double, Tx>* d) {
	vector<float> vf;
	if (d == NULL)
		return -1;
	sortByThroughput(d, vf);
	if (vf.size() == 0)
		return -1;
	else
		return vf[floor(vf.size() * 0.95)];
}
