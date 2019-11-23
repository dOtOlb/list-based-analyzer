#ifndef GeoPerfAnalyzer_Included
#define GeoPerfAnalyzer_Included

#include <vector>
#include <string>
#include <map>
#include <set>
#include "Analyzer.h"
#include "NetworkInfo.h"
#include "TimeSeries.h"
#include "Tx.h"
#include "TxSetCalculator.h"
#include "TimeCurve.h"

using namespace std;

/** 
 * A container analyzer working on the chain model
 *
 */
class GeoPerfAnalyzer: public virtual Analyzer {
public:
	GeoPerfAnalyzer();
	~GeoPerfAnalyzer();

	void initialize(int, char**);
	void initialize(NetworkInfo);
	int parse(vector<string>);
    void analyze();
    void publish();
    string getError();

protected:
	bool isrLine(vector<string>);
	bool isfLine(vector<string>);

	string computeSizeString(int);
	string makeKeyByRegion(string, string, int);
	string makeKeyByGeo(string, string, int);

private:    
    string error_msg;
    NetworkInfo network_info;
    
    set<string> geos;
    set<string> regions;
    float size_buckets[7];
    string size_bucket_names[7];
    map<string, TimeSeries<Tx>> result;
};

#endif