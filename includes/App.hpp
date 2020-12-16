#ifndef App_h
#define App_h
#include "RFM_SENSOR.hpp"
#include "Frame.hpp"
#include "Registry.hpp"
#include "FrameList.hpp"
#include "Helper.hpp"
#include "restclient-cpp/restclient.h"
#include <thread>
#include <algorithm>
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <exception>

#define INITIAL_DATARATE_TX29 17241
#define INITIAL_DATARATE_TX35 9579
#define INITIAL_FREQUENCY 868300
#define STOP "stop"
#define YES "yes"
#define NO "NO"
#define MODE "mode"
#define TX29_MODE "TX29"
#define TX35_MODE "TX35"
#define TOGGLE_MODE "toggle"
#define TOGGLE_TIME "time"
#define DEFAULT_TOGGLE_TIME "15"
#define CYCLE_TIME 100
#define SEND_TO_HOST "-S"
#define HOST "HOST"
#define VERBOSE_MODE "-V"
#define VERBOSE "verbose"

using namespace std;

class Application {
	public:
		int run(int argc, char* argv[]);
	protected:
		Registry registry;
		RFM_SENSOR sensor;
		FrameList list;
		void decodeParams(int argc, char* argv[]);
		static void toggleDataRate(RFM_SENSOR * sensor);
		static void collectDataFrames(RFM_SENSOR * sensor, Registry * registry, FrameList * list);
		static int serveData(Registry * registry, FrameList * list);
		static void serveConnection(Registry * registry, FrameList * list, int socket);
		static void writeToSocket(int socket, string msg);
};

#endif
