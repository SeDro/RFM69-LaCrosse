#ifndef Helper_h
#define Helper_h
#include "Frame.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;

class Helper {
	public:
		static void stringifyFrame(Frame* frame, char* result);
		static bool isNumber(string str);
};
#endif
