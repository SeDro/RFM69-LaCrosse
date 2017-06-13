#ifndef RFM_SENSOR_h
#define RFM_SENSOR_h
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define RF69_MODE_SLEEP       0 // XTAL OFF
#define	RF69_MODE_STANDBY     1 // XTAL ON
#define RF69_MODE_SYNTH	      2 // PLL ON
#define RF69_MODE_RX          3 // RX MODE
#define RF69_MODE_TX		  4 // TX MODE

#define PAYLOADSIZE 64

class RFM_SENSOR
{
	public:
		RFM_SENSOR(void);
		int initializeSensor(unsigned long freq, unsigned long  data_rate);
		void setMode(char newMode);
		bool receiveData(void);
		void getData(unsigned char *data);
		void setDataRate(unsigned long dataRate);
		void setFrequency(unsigned long kHz);
		unsigned long getDataRate(void);
		void printAllRegs(void);
	protected:
		void writeReg(unsigned char addr, unsigned char value);
		unsigned char readReg(unsigned char addr);
		char getDataLen(void);
		void clearFifo(void);
		int isRFM69(void);
		char _mode;       // current transceiver state
		char DATALEN;
		char DATA[PAYLOADSIZE];
		unsigned long m_dataRate;
		unsigned long m_frequency;
};
#endif
