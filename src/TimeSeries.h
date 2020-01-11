#ifndef TimeSeries_Included
#define TimeSeries_Included

using namespace std;

#include <list>
#include <map>
#include <iterator>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <iomanip>

// template definition
template <typename T>
class TimeSeries {
public:

	/**
	 * Constructor (here we have to specify upfront Time Zero, which all time values are relative to)
	 *
	 * @param
	 */
	TimeSeries(double granularity): time_zero(1483228800) {
		this->granularity = granularity;
		this->min_slot = -1;
		this->max_slot = -1;
	}

	/**
	 * Default destructor
	 */
	~TimeSeries()=default;

	/**
	 * the timeToSlot function
	 *
	 * @param time time to be converted from
	 */
	int timeToSlot(double time) {
		if (time < time_zero)
			exit(EXIT_FAILURE);
		return static_cast<int>(ceil((time - time_zero) / granularity));
	}

	/**
	 * the exists function (return true or false)
	 *
	 * @param time
	 * @param obj
	 */
	bool exists(double time, T obj) {
		int point_slot = timeToSlot(time);
		typename map<int, multimap<double, T>>::iterator itr = data.find(point_slot);
		if (itr != data.end()) {
			for (typename multimap<double, T>::iterator itr2 = itr->second.find(time); itr2 != itr->second.end(); itr2++) {
				if ((itr2->second)==obj) {
					return true;
				}
			}
		}
		return false;
	}

	/**
	 * the add function
	 *
	 * @param time
	 * @param obj
	 */
  void add(double time, T obj) {
//		cout << "started: "<< fixed << setprecision(3) << time << "\n";
		if (exists(time, obj)) return;
		int point_slot = timeToSlot(time);
		typename map<int, multimap<double, T>>::iterator itr = data.find(point_slot);
		if (itr == data.end()) {
			multimap<double, T> new_dataset;
			new_dataset.insert(make_pair(time, obj));
			data[point_slot] = new_dataset;

			// update min_slot and max_slot
			if ((this->min_slot == -1) || (this->min_slot > point_slot)) {
				this->min_slot = point_slot;
			}
			if ((this->max_slot == -1) || (this->max_slot < point_slot)) {
				this->max_slot = point_slot;
			}
//			cout << "new add\n";
		}
		else {
			(itr->second).insert(make_pair(time, obj));
//			cout << "existing add\n";
		}
//		cout << "added: "<< fixed << setprecision(3) << time << " " << point_slot << "\n";
	}

	/**
	 * the resetMinMaxSlots function
	 */
	void resetMinMaxSlots() {
		// resetting min and max slots
		int new_min_slot = -1;
		int new_max_slot = -1;
		// loop through by iterator
		for (typename map<int, multimap<double, T>>::iterator itr = data.begin(); itr != data.end(); itr++) {
			if (!(itr->second).empty()) {
				if ((new_min_slot == -1) || (itr->first < new_min_slot))
					new_min_slot = itr->first;
				if ((new_max_slot == -1) || (itr->first > new_max_slot))
					new_max_slot = itr->first;
			}
		}
		this->min_slot = new_min_slot;
		this->max_slot = new_max_slot;
	}

	/**
	 * the remove function
	 *
	 * @param obj
	 * @param begin_time
	 * @param end_time
	 */
  void remove(T obj, double begin_time=0, double end_time=0) {
		// need to define two iterators
		typename TimeSeries<T>::iterator begin_itr = begin_time==0 ? this->begin() : this->begin(begin_time);
		typename TimeSeries<T>::iterator end_itr = end_time==0 ? this->end() : this->end(end_time);

		// keep track of whether we have to reset the min and max slots along the way
		bool slot_reset_needed = false;

		for (; begin_itr != end_itr; begin_itr++) {
			if (begin_itr->datapoints != NULL)
				for (typename multimap<double, T>::iterator itr2 = begin_itr->datapoints->begin(); itr2 != begin_itr->datapoints->end(); itr2++) {
					if ((itr2->second) == obj) {
						begin_itr->datapoints->erase(itr2);

						int point_slot = timeToSlot(begin_itr->begin_time);
						if (begin_itr->datapoints->empty() && ((point_slot < min_slot) || (point_slot > max_slot))) {
							slot_reset_needed = true;
						}

						begin_itr = end_itr;
						break;
					}
				}
		}

		if (slot_reset_needed) resetMinMaxSlots();
	}

	struct timeslot {
		double begin_time;
		double end_time;
		multimap<double, T>* datapoints;
	};

  class iterator;

  iterator begin(double timepoint) {
		return iterator(this, timepoint);
	}

  iterator end(double timepoint) {
		return iterator(this, timepoint);
	}

private:

	double granularity;
	double time_zero;	// Jan 1, 2017 00:00 GMT
	int min_slot;
	int max_slot;

	// use map instead of list for internal storage
	map<int, multimap<double, T>> data;

};

template <typename T>
class TimeSeries<T>::iterator: public std::iterator<
    				std::input_iterator_tag,
    				typename TimeSeries<T>::timeslot,
    				int,
    				typename TimeSeries<T>::timeslot*,
    				typename TimeSeries<T>::timeslot
    					> {
	int slot;
	TimeSeries<T>* parent;
public:
	explicit iterator(TimeSeries<T>* p, double _timepoint = 0) {
		this->parent = p;
		int point_slot = parent->timeToSlot(_timepoint);
		slot = point_slot < parent->min_slot ? parent->min_slot : (point_slot > parent->max_slot ? parent->max_slot+1: point_slot);
	}
	iterator& operator++() {
		slot = slot <= parent->max_slot ? slot+1 : parent->max_slot;
		return *this;
	}
	iterator operator++(int) {
		iterator retval = *this;
		++(*this);
		return retval;
	}
	bool operator==(iterator other) const {
		return slot == other.slot;
	}
	bool operator!=(iterator other) const {
		return !(*this == other);
	}
	typename iterator_traits<iterator>::reference operator*() const {
		multimap<double, T>* data_set;
		typename map<int, multimap<double, T>>::iterator itr = parent->data.find(slot);
		if (itr == parent->data.end())
			data_set = NULL;
		else
			data_set = &(itr->second);

//		cout << "debug 3: " << parent->time_zero << " " << slot << " " << parent->max_slot << " " << parent->min_slot << " " << fixed << setprecision(5) << (slot) * parent->granularity << " " << fixed << setprecision(5) << parent->time_zero + (slot * parent->granularity) << " " << parent->granularity << "\n";

		timeslot current_slot = {
			parent->time_zero + (slot - 1) * parent->granularity, // begin_time
			parent->time_zero + slot * parent->granularity, // end_time
			data_set // datapoints
		};
//		cout << "debug 4: " << current_slot.end_time << "\n";
		return current_slot;

	}
};

#endif
