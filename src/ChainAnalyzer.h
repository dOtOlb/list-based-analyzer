#ifndef ChainAnalyzer_Included
#define ChainAnalyzer_Included

#include <vector>
#include <string>
#include "Analyzer.h"
#include "NetworkInfo.h"
#include <memory>

using namespace std;

/** 
 * A container analyzer working on the chain model
 *
 */
class ChainAnalyzer: public virtual Analyzer {
public:
	ChainAnalyzer();
	~ChainAnalyzer();

	void initialize(int, char**);
	void initialize(NetworkInfo);
	int parseString(string);
	int parse(vector<string>);
    void addAnalyzer();
    void removeAnalyzer();
    void analyze();
    void publish();
    string getError();

protected:

private:    
    vector<std::shared_ptr<Analyzer>> chain;
    int line_count;
    string error_msg;
    NetworkInfo network_info;
};

#endif