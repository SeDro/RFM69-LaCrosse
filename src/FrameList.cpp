#include "FrameList.hpp"

FrameList::FrameList() {
	capacity = 0;
}

void FrameList::addFrame(BaseFrame * theFrame) {
	internal_mutex.lock();
	int tmp = findFrame(theFrame->ID);
	
	if(tmp == -1) {
		cout << "Increase capacity" << endl;
		BaseFrame** theNewList = new BaseFrame*[capacity + 1];
		for(int i = 0; i < capacity; i++) {
			theNewList[i] = theList[i];
		}
//		cout << "List copied" << endl;
		delete theList;
//		cout << "Old list deleted" << endl;
		theList = theNewList;
//		cout << "new pointer set" << endl;
		theList[capacity] = theFrame;
//		cout << "new frame added" << endl;
		capacity++;
//		cout << "capacity increased" << endl;
	}
	else {
		cout << "Starting Update Frame" << endl;
		theList[tmp]->updateFrame(theFrame);
		cout << "Delete Frame" << endl;
		delete theFrame;
		cout << "Deletion finished" << endl;
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

BaseFrame * FrameList::getFrame(unsigned char ID) {
	BaseFrame* ret_val = NULL;
	internal_mutex.lock();
	int tmp = findFrame(ID);
	if(tmp != -1) {
		 ret_val = theList[tmp];
	}
	
	internal_mutex.unlock();
	return ret_val;
}
