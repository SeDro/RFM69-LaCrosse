#include "App.hpp"

void Application::decodeParams(int argc, char* argv[]) {
	registry.setValue(MODE, TOGGLE_MODE);
	registry.setValue(TOGGLE_TIME, DEFAULT_TOGGLE_TIME);

	for(int i = 1; i < argc; i++) {
		string str = argv[i];
		transform(str.begin(), str.end(),str.begin(), ::toupper);
		if(str == TX29_MODE) {
			cout << MODE << " == " << TX29_MODE <<endl;
			registry.setValue(MODE, TX29_MODE);
		}
		else if(str == TX35_MODE) {
			cout << MODE << " == " << TX35_MODE <<endl;
			registry.setValue(MODE, TX35_MODE);
		}
		else if(str == TOGGLE_MODE) {
			cout << MODE << " == " << TOGGLE_MODE <<endl;
			registry.setValue(MODE, TOGGLE_MODE);
		}
		else if(Helper::isNumber(str)) {
			cout << TOGGLE_TIME << " == " << argv[i] <<endl;
			registry.setValue(TOGGLE_TIME, argv[i]);
		}
		else if(str == SEND_TO_HOST) {
			registry.setValue(SEND_TO_HOST, YES);
			cout << SEND_TO_HOST << " is set" << endl;
			if(i + 1 <= argc) { 
				i++;
				cout << HOST << " == " << argv[i] << endl;
				registry.setValue(HOST, argv[i]);
			}
			else {
				cout << SEND_TO_HOST << " need's a host followed." << endl;
				exit(-1);
			}
		}
	}
}

void Application::toggleDataRate(RFM_SENSOR * sensor) {
	unsigned long currentRate = sensor->getDataRate();
	if(currentRate == INITIAL_DATARATE_TX35) {
		sensor->setDataRate(INITIAL_DATARATE_TX29);
	}
	else {
		sensor->setDataRate(INITIAL_DATARATE_TX35);
	}
}

void Application::collectDataFrames(RFM_SENSOR * sensor, Registry* registry, FrameList * list) {
//	try {
	unsigned char received[PAYLOADSIZE];
	int toggle_nr = 0;
//	clock_t startClock, endClock;
	struct timespec pause = {0};
	
	unsigned long initialRate = INITIAL_DATARATE_TX29;
	if(registry->getValue(MODE) == TX35_MODE)
		initialRate = INITIAL_DATARATE_TX35;
	int err = sensor->initializeSensor(INITIAL_FREQUENCY, initialRate);
  
	if(err != 0) {
		return;
	}
	sensor->setMode(RF69_MODE_RX);
	
	string stop = registry->getValue(STOP);
	while(stop.empty() || stop != YES) {
//		startClock = clock();
		if(sensor->receiveData() == true) {
			sensor->setMode(RF69_MODE_STANDBY);
			sensor->getData(received);
			char json[1000];
			Frame * frame = Frame::decodeFrame(received);
			if(frame != nullptr) {
//				cout << "try stringify frame" << endl;
//				cout << "try add frame" << endl;
				list->addFrame(frame);
				if(registry->getValue(SEND_TO_HOST) == YES) {
					Helper::stringifyFrame(frame, json);
					RestClient::Response r = RestClient::post(registry->getValue(HOST), "application/json", json);
				}
				frame = nullptr;
			}
			sensor->setMode(RF69_MODE_RX);
		}

		toggle_nr++;
		if(registry->getValue(MODE) == TOGGLE_MODE && toggle_nr * CYCLE_TIME / 1000 >= stoi (registry->getValue(TOGGLE_TIME),nullptr,0)) {
			toggleDataRate(sensor);
			toggle_nr = 0;
		}
	
		stop = registry->getValue(STOP);
//		endClock = clock();
	
		pause.tv_nsec = 50000000;//CYCLE_TIME * 1000000L - (endClock - startClock) * 1000;
		nanosleep(&pause, (struct timespec *)nullptr);
	}
/*	} catch (exception& e) {
		cout << "Exception during collectDataFrames: " << e.what() << endl;
	} catch (...) {
		cout << "undefined exception in collectDataFrames" << endl;
	}
*/}

int Application::run(int argc, char* argv[]) {

	cout << "Application starts with " << argc << " arguments" << endl;
	decodeParams(argc, argv);
	
//	std::thread collecting(collectDataFrames, &sensor, &registry, &list);
	std::thread serving(serveData, &registry, &list);
	
	collectDataFrames(&sensor, &registry, &list);
	
//	collecting.join();
	serving.join();
	
	return 0;
}

int Application::serveData(Registry * registry, FrameList * list) {
	
//	try {
	int sockfd, newsockfd, portno, pid;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	fcntl(sockfd, F_SETFL, O_NONBLOCK);
	if(sockfd < 0) {
		registry->setValue(STOP, YES);
		return -1;
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = 8888;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		registry->setValue(STOP, YES);
		return -2;
	}
	if(listen(sockfd,5) < 0) {
		registry->setValue(STOP, YES);
		return -4;
	}
	clilen = sizeof(cli_addr);
	string stop = registry->getValue(STOP);
	while(stop.empty() || stop != YES) {
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0) {
			if(errno == EAGAIN || errno == EWOULDBLOCK) {
				struct timespec pause = {0};
				pause.tv_nsec = 1000000L;
				nanosleep(&pause, (struct timespec *)nullptr);
				stop = registry->getValue(STOP);
				continue;
			}
			else {
				cout << "Error at accepting new connection. Errno: " << errno << endl;
				registry->setValue(STOP, YES);
				return -3;
			}
		}
//		try {
			std::thread handleConnection(serveConnection, registry, list, newsockfd);
			handleConnection.detach();
/*		} catch (exception& e) {
			cout << "Exception during creating new thread: " << e.what() << endl;
		}
*/		stop = registry->getValue(STOP);
	}
	close(sockfd);
	return 0;
/*	}catch (...) {
		cout << "undefined exception in serveData" << endl;
	}
*/}


void Application::serveConnection(Registry * registry, FrameList * list, int socket) {
	char buffer[10];
	
//	try {
		bzero(buffer, 10);
		read(socket, buffer, 9);
	
		string str = buffer;
		transform(str.begin(), str.end(),str.begin(), ::toupper);
		string stop = STOP;
		transform(stop.begin(), stop.end(),stop.begin(), ::toupper);
		if(str.length() >= stop.length() && std::equal(stop.begin(), stop.end(), str.begin())) {
			registry->setValue(STOP, YES);
		}
		else {
			int tmp = strtol (str.data(),nullptr,0);
			Frame * requestedFrame = (Frame*)list->getFrame(tmp);
			if(requestedFrame != nullptr) {
				writeToSocket(socket, requestedFrame->NewBatteryFlag == true ? "TRUE" : "FALSE");
				writeToSocket(socket, requestedFrame->Bit12 == true ? "TRUE" : "FALSE");
				snprintf(buffer, 10, "%.1f", requestedFrame->Temperature);
				writeToSocket(socket, buffer);
				writeToSocket(socket, requestedFrame->WeakBatteryFlag == true ? "TRUE" : "FALSE");
				snprintf(buffer, 10, "%02i\%", requestedFrame->Humidity);
				writeToSocket(socket, buffer);
				snprintf(buffer, 10, "%f", requestedFrame->HumidityAbs);
				writeToSocket(socket, buffer);
				requestedFrame = nullptr;
 			}
			else {
				writeToSocket(socket, str);
				writeToSocket(socket, "ID not found");
   			}
		}
		close(socket);
/*	} catch (exception& e) {
		cout << "Exception during serveConnection: " << e.what() << endl;
	} catch (...) {
		cout << "undefined exception in serveConnection" << endl;
	}
*/}

void Application::writeToSocket(int socket, string msg) {
	write(socket, msg.data(), msg.size());
	msg = ";";
	write(socket,  msg.data(), msg.size()); 
}
