#ifndef BaseFrame_h
#define BaseFrame_h
#include <stdio.h>
#include <iostream>
using namespace std;

class BaseFrame {
	public:
	    unsigned char  Header;
		unsigned char  ID;
		virtual void updateFrame(BaseFrame* newFrame) {
			this->Header = newFrame->Header;
			this->ID = newFrame->ID;
		}
		BaseFrame() {
			cout << "BaseFrame created" << endl;
		}
		
		virtual ~BaseFrame() {
			cout << "Base deleted" << endl;
		}
};
#endif
