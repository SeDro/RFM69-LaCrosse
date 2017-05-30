#ifndef FrameList_h
#define FrameList_h
#include "Frame.hpp"
#include <stdio.h>
#include <iostream>
#include <mutex>

using namespace std;

class FrameList {
	public:
		void addFrame(Frame * newFrame);
		Frame * getFrame(unsigned char ID);
		FrameList();
	protected:
		Frame** theList;
		int capacity;
		int findFrame(unsigned char ID);
		std::mutex internal_mutex;
};

#endif
