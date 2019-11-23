#ifndef TxSetCalculator_Included
#define TxSetCalculator_Included

#include "Tx.h"
#include "TimeSeries.h"
#include <map>
#include <vector>

using namespace std;

class TxSetCalculator {
public:

	enum TxSetAction { AVERAGE_1, AVERAGE_2, MEDIAN, PERCENTILE_95 };
	
	TxSetCalculator(TxSetAction a);
	~TxSetCalculator()=default;

	float operator() (TimeSeries<Tx>::timeslot ts);

protected:

	float findAverage1(multimap<double, Tx>* d);
	float findAverage2(multimap<double, Tx>* d);
	float findMedian(multimap<double, Tx>* d);
	float find95Percentile(multimap<double, Tx>* d);
	void sortByThroughput(multimap<double, Tx>* d, vector<float>& vf);

private:

	TxSetAction action;
};

#endif