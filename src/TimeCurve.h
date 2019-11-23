#ifndef TimeCurve_Included
#define TimeCurve_Included

#include "TimeSeries.h"
#include "TxSetCalculator.h"

using namespace std;

class TimeCurve {
public:

	~TimeCurve()=default;

	TimeCurve(TimeSeries<Tx>::iterator, TimeSeries<Tx>::iterator, TxSetCalculator);
	
	map<double, float> getCurve() const;
	int getFullCount() const;
	int getCount() const;
	float getMax() const;
	float getMin() const;
	float getAvg() const;
	double getStartTime() const;
	double getEndTime() const;
	double getGranularity() const;	
	
	string toString() const;

protected:


private:

	TimeCurve()=default;	

	map<double,float> curve;
	int full_count;
	int count;
	float max;
	float min;
	float avg;
	double start_time;
	double end_time;
	double granularity;
};

#endif