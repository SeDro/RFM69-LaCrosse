#ifndef RFM69_h
#define RFM69_h

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

int rfm69_initialize(unsigned long freq, unsigned long  data_rate);
void rfm69_writeReg(unsigned char addr, unsigned char value);
unsigned char rfm69_readReg(unsigned char addr);
void rfm69_readAllRegs(void);
void rfm69_setMode(char newMode);
bool rfm69_receive(void);
char rfm69_getDataLen(void);
void rfm69_getData(unsigned char *data);
void SetDataRate(unsigned long dataRate);
void SetFrequency(unsigned long kHz);
void ClearFifo();
int IsRFM69();
unsigned long GetDataRate();

#endif
