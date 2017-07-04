#include "RFM_SENSOR.hpp"
#include "Frame.hpp"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>

#define INITIAL_DATARATE_TX29 17241
#define INITIAL_FREQUENCY 868300
#define INITIAL_DATARATE_TX35 9579

using namespace std;

int main(int argc, char* argv[]) {
	RFM_SENSOR sensor;
	int err = sensor.initializeSensor(INITIAL_FREQUENCY, INITIAL_DATARATE_TX35);
  
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
					myfile << to_string(frame->Header) << endl;
					myfile << to_string(frame->ID) << endl;
					myfile << to_string(frame->NewBatteryFlag) << endl;
					myfile << to_string(frame->Bit12) << endl;
					myfile << to_string(frame->Temperature) << endl;
					myfile << to_string(frame->WeakBatteryFlag) << endl;
					myfile << to_string(frame->Humidity) << endl;
					myfile << to_string(frame->CRC) << endl;
					myfile << to_string(frame->HumidityAbs) << endl;
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
