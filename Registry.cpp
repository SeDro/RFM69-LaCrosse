#include "Registry.hpp"

bool Registry::setValue(string key, string value) {
	internal_mutex.lock();

	auto tmp = internal_map.find(key);
	if(tmp != internal_map.end()) {
		internal_map.erase(tmp);
	}
	auto ret_val = internal_map.emplace(key, value);

	internal_mutex.unlock();
	
	return get<1>(ret_val);
}

string Registry::getValue(string key) {
	string ret_val;
	internal_mutex.lock();
	auto tmp = internal_map.find(key);
	if(tmp != internal_map.end()) {
		ret_val = tmp->second;
	}
	internal_mutex.unlock();
	
	return ret_val;
}
