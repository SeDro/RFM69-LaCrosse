#ifndef FrameList_h
#define FrameList_h
#include "Frame.hpp"
#include <stdio.h>
#include <iostream>
#include <mutex>

using namespace std;

class FrameList {
	public:
		void addFrame(BaseFrame * newFrame);
		BaseFrame * getFrame(unsigned char ID);
		FrameList();
	protected:
		BaseFrame** theList = nullptr;
		int capacity;
		int findFrame(unsigned char ID);
		std::mutex internal_mutex;
};

#endif
