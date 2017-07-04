#include <fstream>
#include <iostream>
#include <stdio.h>
#include "Frame.hpp"
#include "RFM_SENSOR.hpp"
#include <random>

#define NUMBER_OF_TESTS 5

using namespace std;
class TestFrame : public Frame {
	public:
		unsigned char Valid = 0;
		unsigned char Data[PAYLOADSIZE];		
};

void Assert(bool assertion, string err_msg) {
	if(assertion == false) {
		cout << err_msg;
		exit(-1);
	}
}

int main(int argc, char* argv[]) {
	TestFrame testees[NUMBER_OF_TESTS];
	char buffer[10];
	char tmpc[20];

	for(int i = 0; i < NUMBER_OF_TESTS; i++) {
		string file_name = "values_";
		file_name += to_string(i);
		file_name += ".bin";
		cout << file_name << endl;

		ifstream myfile (file_name);
		if (myfile.is_open())
		{
			myfile >> buffer;
			cout << buffer << endl;
			string str = buffer;
			if(str == "Valid") {
				testees[i].Valid = 1;
				myfile >> tmpc;
				testees[i].Header =  atoi(tmpc);
				myfile >> tmpc;
				testees[i].ID =  atoi(tmpc);
				myfile >> tmpc;
				testees[i].NewBatteryFlag =  atoi(tmpc);
				myfile >> tmpc;
				testees[i].Bit12 =  atoi(tmpc);
				myfile >> tmpc;
				testees[i].Temperature =  atof(tmpc);
				myfile >> tmpc;
				testees[i].WeakBatteryFlag =  atoi(tmpc);
				myfile >> tmpc;
				testees[i].Humidity =  atoi(tmpc);
				myfile >> tmpc;
				testees[i].CRC =  atoi(tmpc);
				myfile >> tmpc;
				testees[i].HumidityAbs =  atof(tmpc);
			}
			myfile >> testees[i].Data;
			myfile.close();
		}
	}
  
	default_random_engine generator;
	uniform_int_distribution<int> distribution(0,NUMBER_OF_TESTS - 1);
	for (long long i = 0; i < 10000000; i++) {
		int number = distribution(generator);
		cout << "test number: " << i << " testee: " << number << endl;
		Frame * test = Frame::decodeFrame(testees[number].Data);
		Assert((testees[number].Valid == 0 && test == NULL) || (testees[number].Valid == 1 && test != NULL), "Error in decode " + to_string(number) + " validitiy expected: " + to_string(testees[number].Valid) + "\n");
		if(testees[number].Valid == 1) {
			Assert(testees[number].Header == test->Header, "Header expected: " + to_string(testees[number].Header) + "but was: " + to_string(test->Header) + "\n");
			Assert(testees[number].ID == test->ID, "ID expected: " + to_string(testees[number].ID) + "but was: " + to_string(test->ID) + "\n");
			Assert(testees[number].NewBatteryFlag == test->NewBatteryFlag, "NewBatteryFlag expected: " + to_string(testees[number].NewBatteryFlag) + "but was: " + to_string(test->NewBatteryFlag) + "\n");
			Assert(testees[number].Bit12 == test->Bit12, "Bit12 expected: " + to_string(testees[number].Bit12) + "but was: " + to_string(test->Bit12) + "\n");
			Assert(testees[number].Temperature == test->Temperature, "Temperature expected: " + to_string(testees[number].Temperature) + "but was: " + to_string(test->Temperature) + "\n");
			Assert(testees[number].WeakBatteryFlag == test->WeakBatteryFlag, "WeakBatteryFlag expected: " + to_string(testees[number].WeakBatteryFlag) + "but was: " + to_string(test->WeakBatteryFlag) + "\n");
			Assert(testees[number].Humidity == test->Humidity, "Humidity expected: " + to_string(testees[number].Humidity) + "but was: " + to_string(test->Humidity) + "\n");
			Assert(testees[number].CRC == test->CRC, "CRC expected: " + to_string(testees[number].CRC) + "but was: " + to_string(test->CRC) + "\n");
			Assert(to_string(testees[number].HumidityAbs) == to_string(test->HumidityAbs), "HumidityAbs expected: " + to_string(testees[number].HumidityAbs) + "but was: " + to_string(test->HumidityAbs) + "\n");
			delete test;
		}
	}
 }
