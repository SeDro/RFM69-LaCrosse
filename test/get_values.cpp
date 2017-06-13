#include "RFM_SENSOR.hpp"
#include "Frame.hpp"
#include <fstream>
#include <iostream>
#include <stdio.h>

#define INITIAL_DATARATE_TX29 17241
#define INITIAL_FREQUENCY 868300

using namespace std;

int main(int argc, char* argv[]) {
	RFM_SENSOR sensor;
	int err = sensor.initializeSensor(INITIAL_FREQUENCY, INITIAL_DATARATE_TX29);
  
	if(err != 0) {
		return err;
	}
	sensor.setMode(RF69_MODE_RX);

	int valueCounter = 0;
	
	unsigned char received[PAYLOADSIZE];
	
	while(valueCounter < 5) {
		if(sensor.receiveData() == true) {
			sensor.setMode(RF69_MODE_STANDBY);
			sensor.getData(received);
			char buffer[20];
			
			snprintf(buffer, 20, "values_%i.bin", valueCounter);

			ofstream myfile (buffer);
			if (myfile.is_open())
			{
				Frame * frame = Frame::decodeFrame(received);
				if(frame != NULL) {
					myfile << "Valid" << endl;
					myfile << frame->Header << "\n";
					myfile << frame->ID << endl;
					myfile << frame->NewBatteryFlag << endl;
					myfile << frame->Bit12 << endl;
					myfile << frame->Temperature << endl;
					myfile << frame->WeakBatteryFlag << endl;
					myfile << frame->Humidity << endl;
					myfile << frame->CRC << endl;
					myfile << frame->HumidityAbs << endl;
				}
				else {
					myfile << "Invalid" << endl;
				}
				myfile.write((const char *)received, PAYLOADSIZE);
				myfile.flush();
				myfile.close();
				valueCounter++;
			}
			sensor.setMode(RF69_MODE_RX);
		}

	}
}
