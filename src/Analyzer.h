#ifndef Analyzer_Included
#define Analyzer_Included

#include <string>
#include <vector>
#include "NetworkInfo.h"

using namespace std;

/** 
 * Virtual base class for all analyzers
 *
 */
class Analyzer {
public:
	enum AnalyzeStatus { ALL_GOOD, UNKNOWN_LOG_FORMAT };
	
	Analyzer()=default;
	~Analyzer()=default;
	
	void initialize(int argc, char** argv) {}
	void initialize(NetworkInfo n) {}
	virtual int parse(vector<string>)=0;
    virtual void analyze()=0;
    virtual void publish()=0;
    virtual string getError()=0;
    string getName() {
    	return this->name;
    }
    
private:
	string name;
};

#endif