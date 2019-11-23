#ifndef NetworkInfo_Included
#define NetworkInfo_Included

#include <string>
#include <map>
#include "utilities.h"

using namespace std;

struct NetworkInfo {
	map<string, string> IpToRegion;
	map<string, string> RegionToGeo;
	
	bool isEmpty() {
		return (IpToRegion.empty() || RegionToGeo.empty());
	}
	
	void initIpToRegion(string csv_file) {
		readCSVFile<string, string>(csv_file, IpToRegion);
	}

	void initRegionToGeo(string csv_file) {
		readCSVFile<string, string>(csv_file, RegionToGeo);
	}
	
	string getRegion(string IP) {
		map<string, string>::iterator itr = IpToRegion.find(IP);
		
		if (itr != IpToRegion.end()) {
			return itr->second;
		}
		return "";
	}
	
	string getGeo(string region) {
		map<string, string>::iterator itr = RegionToGeo.find(region);
		
		if (itr != RegionToGeo.end()) {
			return itr->second;
		}
		return "";
	}
};

#endif