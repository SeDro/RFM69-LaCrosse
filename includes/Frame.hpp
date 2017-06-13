#ifndef Frame_h
#define Frame_h
#include <stdio.h>
#include <iostream>

#define FRAME_LENGTH 5

using namespace std;

class Frame {
	public:
	    unsigned char  Header;
		unsigned char  ID;
		unsigned char  NewBatteryFlag;
		unsigned char  Bit12;
		float Temperature;
		unsigned char  WeakBatteryFlag;
		unsigned char  Humidity;
		unsigned char  CRC;
		double HumidityAbs;
		static Frame * decodeFrame(unsigned char *bytes);
		~Frame();
	protected:
		static unsigned char CalculateCRC(unsigned char *data, unsigned char len);
		Frame();
};

#endif
