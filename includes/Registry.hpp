#ifndef Registry_h
#define Registry_h
#include <stdio.h>
#include <mutex>
#include <string>
#include <map>

using namespace std;

class Registry {
	public:
		bool setValue(string key, string value);
		string getValue(string key);
	protected:
		std::mutex internal_mutex;
		map<string, string> internal_map;
};

#endif
