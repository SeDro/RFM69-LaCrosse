#include "FrameList.hpp"

FrameList::FrameList() {
	capacity = 0;
}

void FrameList::addFrame(Frame * theFrame) {
	internal_mutex.lock();
	int tmp = findFrame(theFrame->ID);
	
	if(tmp == -1) {
		Frame** theNewList = new Frame*[capacity + 1];
		for(int i = 0; i < capacity; i++) {
			theNewList[i] = theList[i];
		}
		delete[] theList;
		theList = theNewList;
		theList[capacity] = theFrame;
		capacity++;
	}
	else {
		delete[] theList[tmp];
		theList[tmp] = theFrame;
	}
	
	internal_mutex.unlock();
}

int FrameList::findFrame(unsigned char ID) {
	int tmp = -1;

	for(int i = 0; i < capacity; i++) {
		if(theList[i]->ID == ID) {
			tmp = i;
			break;
		}
	}
	return tmp;
}

Frame * FrameList::getFrame(unsigned char ID) {
	Frame* ret_val = NULL;
	internal_mutex.lock();
	int tmp = findFrame(ID);
	if(tmp != -1) {
		 ret_val = theList[tmp];
	}
	
	internal_mutex.unlock();
	return ret_val;
}
