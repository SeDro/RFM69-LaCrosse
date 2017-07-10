#include <stdio.h>
#include <random>
#include "FrameList.hpp"
#include "BaseFrame.hpp"

void Assert(bool assertion, string err_msg) {
	if(assertion == false) {
		cout << err_msg;
		exit(-1);
	}
}

int main(int argc, char* argv[]) {
	FrameList list;
	default_random_engine generator;
	default_random_engine generator2;
	uniform_int_distribution<int> distribution(0,255);
	uniform_int_distribution<int> distribution2(0,255);

	for (long long i = 0; i < 1000; i++) {
		unsigned char ID = distribution(generator);
		unsigned char Header = distribution2(generator2);
		BaseFrame* theFrame = new BaseFrame();
		theFrame->ID = ID;
		theFrame->Header = Header;
		list.addFrame(theFrame);
		cout << "addFrame finished"<<endl;
		theFrame = NULL;
		theFrame = list.getFrame(ID);
		Assert(theFrame != NULL, "Did not found Frame header " + to_string(ID) + " turn " + to_string(i) + "\n");
		Assert(theFrame->ID == ID, "Difference in ID, expected " + to_string(ID)  + " but was " + to_string(theFrame->ID) + " turn " + to_string(i) + "\n");
		Assert(theFrame->Header == Header, "Difference in Header, expected " + to_string(Header)  + " but was " + to_string(theFrame->Header) + " turn " + to_string(i) + "\n");
	}
}
