#include "Frame.hpp"
#include <math.h>

unsigned char Frame::CalculateCRC(unsigned char *data, unsigned char len) {
  int i, j;
  unsigned char res = 0;
  for (j = 0; j < len; j++) {
    unsigned char val = data[j];
    for (i = 0; i < 8; i++) {
      unsigned char tmp = (unsigned char)((res ^ val) & 0x80);
      res <<= 1;
      if (0 != tmp) {
        res ^= 0x31;
      }
      val <<= 1;
    }
  }
  return res;
}

Frame * Frame::decodeFrame(unsigned char *bytes) {
  Frame * frame = nullptr;

  if (bytes[4] == CalculateCRC(bytes, FRAME_LENGTH - 1)) {
		frame = new Frame();

		frame->Header = (bytes[0] & 0xF0) >> 4;
		if (frame->Header != 9) {
			cout << "wrong header " << frame->Header <<endl;
			delete frame;
			frame = nullptr;
		}
		else {
			frame->CRC = bytes[4];
			// SSSS.DDDD DDN_.TTTT TTTT.TTTT WHHH.HHHH CCCC.CCCC
			frame->ID = 0;
			frame->ID |= (bytes[0] & 0xF) << 2;


			frame->NewBatteryFlag = (bytes[1] & 0x20) >> 5;

			frame->Bit12 = (bytes[1] & 0x10) >> 4;

			unsigned char bcd[3];
			bcd[0] = bytes[1] & 0xF;
			bcd[1] = (bytes[2] & 0xF0) >> 4;
			bcd[2] = (bytes[2] & 0xF);
			float t = 0;
			t += bcd[0] * 100.0;
			t += bcd[1] * 10.0;
			t += bcd[2] * 1.0;
			t = t / 10;
			t -= 40;
			frame->Temperature = t;

			frame->WeakBatteryFlag = (bytes[3] & 0x80) >> 7;

			frame->Humidity = bytes[3] & 0b01111111;

			double sdd, dd;
			sdd=6.1078 * pow(10,(7.5*frame->Temperature)/(237.3+frame->Temperature));
			dd=frame->Humidity/100.0*sdd;
			frame->HumidityAbs = 216.687*dd/(273.15+frame->Temperature);
		}
	} else {
		cout << "wrong CRC" << endl;
	}
  return frame;
}

void Frame::updateFrame(BaseFrame* newFrame) {
	cout << "updating Frame " << (unsigned int)this->ID << endl;
	BaseFrame::updateFrame(newFrame);
	cout << "updating BaseFrame finished" << endl;
	Frame* tmp = dynamic_cast<Frame*>(newFrame);
	if(tmp != nullptr) {
		cout << "update Frame data" << endl;
		this->CRC = tmp->CRC;
		this->NewBatteryFlag = tmp->NewBatteryFlag;
		this->Bit12 = tmp->Bit12;
		this->Temperature = tmp->Temperature;
		this->WeakBatteryFlag = tmp->WeakBatteryFlag;
		this->Humidity = tmp->Humidity;
		this->HumidityAbs = tmp->HumidityAbs;
		cout << "update Frame data finished" << endl;
	}
}

Frame::Frame() {
	cout << "Frame created" << endl;
}

Frame::~Frame() {
	cout << "Frame deleted" << endl;
}
