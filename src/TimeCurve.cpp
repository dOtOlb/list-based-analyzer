#include "TimeCurve.h"
#include <sstream>
#include <iomanip>

using namespace std;

/**
 * Constructor
 *
 * @param
 */
TimeCurve::TimeCurve(TimeSeries<Tx>::iterator begin_itr, TimeSeries<Tx>::iterator end_itr, TxSetCalculator calc) {
	float res_val;

	bool first = true;
	full_count = 0;
	count = 0;

	for (; begin_itr != end_itr; begin_itr++) {
		res_val=calc(*begin_itr);
		curve[(*begin_itr).end_time]=res_val;
//		cout << "debug 2: " << (*begin_itr).end_time << "\n";

//		cout << res_val << "\n";

		granularity = (*begin_itr).end_time - (*begin_itr).begin_time;
		if (first || ((*begin_itr).begin_time < start_time)) start_time = (*begin_itr).begin_time;
		if (first || ((*begin_itr).end_time > end_time)) end_time = (*begin_itr).end_time;

		full_count++;

//		cout << "debug: " << full_count << "\n";

		if (res_val != -1) {
			count++;
			if (first || (res_val < min)) min = res_val;
			if (first || (res_val > max)) max = res_val;
			avg = first ? res_val : avg + res_val;
		}

		first = false;
	}
//	cout << count << " " << full_count << "\n";
	avg /= count;
}

/**
 * The getFullCount function (const)
 *
 * @param
 */
int TimeCurve::getFullCount() const {
	return full_count;
}

/**
 * The getCount function (const)
 *
 * @param
 */
int TimeCurve::getCount() const {
	return count;
}

/**
 * The getMax function (const)
 *
 * @param
 */
float TimeCurve::getMax() const {
	return max;
}

float TimeCurve::getMin() const {
	return min;
}

float TimeCurve::getAvg() const {
	return avg;
}

double TimeCurve::getStartTime() const {
	return start_time;
}

double TimeCurve::getEndTime() const {
	return end_time;
}

double TimeCurve::getGranularity() const {
	return granularity;
}

string TimeCurve::toString() const {
	stringstream output_ss;
	output_ss <<
	"Max: " << fixed << setprecision(2) << getMax() << "\n" <<
	"Min: " << fixed << setprecision(2) << getMin() << "\n" <<
	"Avg: " << fixed << setprecision(2) << getAvg() << "\n" <<
	"Data:\n";

	for (map<double, float>::const_iterator itr = curve.begin(); itr != curve.end(); itr++) {
		output_ss << fixed << setprecision(0) << itr->first << "," << setprecision(2) << itr->second << "\n";
	}

	return output_ss.str();
}
