#ifndef Utilities_Included
#define Utilities_Included

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <map>

using namespace std;

// this file has some of the most commonly used utility functions defined for the program

/**
 * Utility function to tokenize string (credit to www.cplusplus.com)
 *
 * @param s
 * @param delimiters
 */
static vector<string> tokenize(string const s, string const delimiters=" ") {
	vector<string> vs;
	size_t current;
	size_t next = -1;
	do {
		current = next + 1;
		next = s.find_first_of(delimiters, current);
		vs.push_back(s.substr(current, next - current));
	} while (next != string::npos);
	return vs;
}

template <typename T>
struct TypedExtractor
{
   static T get(std::string const& s)
   {
      T out;
      std::stringstream ss(s);
      ss >> out;
      return out;
   }
};

template <>
struct TypedExtractor<std::string>
{
   static std::string get(std::string const& s)
   {
      return s;
   }
};

/**
 * The helper function to read/initialize a CSV file into a map.
 * It does not use the tokenize function.
 * credit: www.stackoverflow.com
 *
 * @param
 */
template <typename A, typename B>
void readCSVFile(string map_file, map<A, B> &map_table) {
	A key;
	B value;
	string key_string, value_string;
	stringstream ss;

	// check if the file is empty
	if (map_file.empty()) {
		cerr << "Error: empty file name!" << endl;
		exit(EXIT_FAILURE);
	}

	// check if we can open the file
	ifstream input_stream(map_file);
	if (!input_stream.is_open()) {
		cerr << "Error: file read error!" << endl;
		exit(EXIT_FAILURE);
	}

	// read the data line by line into the map container
	while (std::getline(input_stream, key_string, ',') && std::getline(input_stream, value_string, ',')) {
		key = TypedExtractor<A>::get(key_string);
		value = TypedExtractor<B>::get(value_string);
		map_table[key] = value;
	}

	// close the stream
	input_stream.close();
}

#endif
