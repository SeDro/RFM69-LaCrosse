#include "App.hpp"


int main(int argc, char* argv[]) {
	Application theApp;
	try {
	theApp.run(argc, argv);
	} catch (...) {
		cout << "undefined exception in theApp.run" << endl;
	}
};
