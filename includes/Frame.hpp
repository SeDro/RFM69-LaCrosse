#ifndef Frame_h
#define Frame_h
#include "BaseFrame.hpp"

#define FRAME_LENGTH 5

class Frame : public BaseFrame {
	public:
		unsigned char  NewBatteryFlag;
		unsigned char  Bit12;
		float Temperature;
		unsigned char  WeakBatteryFlag;
		unsigned char  Humidity;
		unsigned char  CRC;
		double HumidityAbs;
		static Frame * decodeFrame(unsigned char *bytes);
		virtual ~Frame();
		virtual void updateFrame(BaseFrame* newFrame);
		Frame();
	protected:
		static unsigned char CalculateCRC(unsigned char *data, unsigned char len);
};

#endif
