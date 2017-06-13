#include <fstream>
#include <iostream>
#include <stdio.h>
using namespace std;

int main(int argc, char* argv[]) {
	char buffer[10];
	unsigned char tmpc;

	ifstream myfile ("values_1.bin");
	if (myfile.is_open())
	{
		myfile >> buffer;
		cout << buffer << endl;
		string str = buffer;
		if(str == "Valid") {
			myfile >> tmpc;
			cout << tmpc << endl;
		}
		myfile.close();
	}
}
