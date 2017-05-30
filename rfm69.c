#include "rfm69.h"
#include "rfm69registers.h"
#include <wiringPiSPI.h>

#define SPI_SPEED 2000000
#define SPI_DEVICE 0

char _mode;       // current transceiver state
char DATALEN;
char DATA[PAYLOADSIZE];
unsigned long m_dataRate;
unsigned long m_frequency;


int rfm69_initialize(unsigned long  freq, unsigned long  data_rate) {

  // Initialize SPI device 0
  if(wiringPiSPISetup(SPI_DEVICE, SPI_SPEED) < 0) {
    printf("Unable to open SPI device\n\r");
    exit(1);
  }
  
  if(!IsRFM69()) {
	  printf("SPI device is no RFM69\n\r");
	  exit(2);
  }

  do rfm69_writeReg(REG_SYNCVALUE1, 0xaa); while(rfm69_readReg(REG_SYNCVALUE1) != 0xaa);
  do rfm69_writeReg(REG_SYNCVALUE1, 0x55); while(rfm69_readReg(REG_SYNCVALUE1) != 0x55);

  rfm69_setMode(RF69_MODE_STANDBY);

    /* 0x01 */ rfm69_writeReg(REG_OPMODE, RF_OPMODE_SEQUENCER_ON | RF_OPMODE_LISTEN_OFF | RF_OPMODE_STANDBY);
    /* 0x02 */ rfm69_writeReg(REG_DATAMODUL, RF_DATAMODUL_DATAMODE_PACKET | RF_DATAMODUL_MODULATIONTYPE_FSK | RF_DATAMODUL_MODULATIONSHAPING_00);
    /* 0x05 */ rfm69_writeReg(REG_FDEVMSB, RF_FDEVMSB_90000);
    /* 0x06 */ rfm69_writeReg(REG_FDEVLSB, RF_FDEVLSB_90000);
    /* 0x11 */ rfm69_writeReg(REG_PALEVEL, RF_PALEVEL_PA0_ON | RF_PALEVEL_PA1_OFF | RF_PALEVEL_PA2_OFF | RF_PALEVEL_OUTPUTPOWER_11111);
    /* 0x13 */ rfm69_writeReg(REG_OCP, RF_OCP_OFF);
    /* 0x19 */ rfm69_writeReg(REG_RXBW, RF_RXBW_DCCFREQ_010 | RF_RXBW_MANT_16 | RF_RXBW_EXP_2);
    /* 0x28 */ rfm69_writeReg(REG_IRQFLAGS2, RF_IRQFLAGS2_FIFOOVERRUN);
    /* 0x29 */ rfm69_writeReg(REG_RSSITHRESH, 220);
    /* 0x2E */ rfm69_writeReg(REG_SYNCCONFIG, RF_SYNC_ON | RF_SYNC_FIFOFILL_AUTO | RF_SYNC_SIZE_2 | RF_SYNC_TOL_0);
    /* 0x2F */ rfm69_writeReg(REG_SYNCVALUE1, 0x2D);
    /* 0x30 */ rfm69_writeReg(REG_SYNCVALUE2, 0xD4);
    /* 0x37 */ rfm69_writeReg(REG_PACKETCONFIG1, RF_PACKET1_CRCAUTOCLEAR_OFF);
    /* 0x38 */ rfm69_writeReg(REG_PAYLOADLENGTH, PAYLOADSIZE);
    /* 0x3C */ rfm69_writeReg(REG_FIFOTHRESH, RF_FIFOTHRESH_TXSTART_FIFONOTEMPTY | RF_FIFOTHRESH_VALUE);
    /* 0x3D */ rfm69_writeReg(REG_PACKETCONFIG2, RF_PACKET2_RXRESTARTDELAY_2BITS | RF_PACKET2_AUTORXRESTART_ON | RF_PACKET2_AES_OFF);
    /* 0x6F */ rfm69_writeReg(REG_TESTDAGC, RF_DAGC_IMPROVED_LOWBETA0);

  SetFrequency(freq);
  SetDataRate(data_rate);

  ClearFifo();
  
  return 0;
}

void SetDataRate(unsigned long dataRate) {
  m_dataRate = dataRate;

  int r = ((32000000UL + (m_dataRate / 2)) / m_dataRate);
  rfm69_writeReg(REG_BITRATEMSB, r >> 8);
  rfm69_writeReg(REG_BITRATELSB, r & 0xFF);
}

unsigned long rfm_GetDataRate() {
	return m_dataRate;
}

void SetFrequency(unsigned long kHz) {
  m_frequency = kHz;
  unsigned long f = (((kHz * 1000) << 2) / (32000000L >> 11)) << 6;
  rfm69_writeReg(REG_FRFMSB, f >> 16);
  rfm69_writeReg(REG_FRFMID, f >> 8);
  rfm69_writeReg(REG_FRFLSB, f);
}

void ClearFifo() {
  rfm69_writeReg(REG_IRQFLAGS2, 16);
}

int IsRFM69()
{
	  // Is there a RFM69 ?
  rfm69_writeReg(REG_PAYLOADLENGTH, 0xA);
  if (rfm69_readReg(REG_PAYLOADLENGTH) == 0xA) {
    rfm69_writeReg(REG_PAYLOADLENGTH, 0x40);
    if (rfm69_readReg(REG_PAYLOADLENGTH) == 0x40) {
      return 1;
    }
  }
  return 0;
}

void rfm69_writeReg(unsigned char addr, unsigned char value) {
  unsigned char thedata[2];
  thedata[0] = addr | 0x80;
  thedata[1] = value;

  wiringPiSPIDataRW(SPI_DEVICE, thedata, 2);
  usleep(5);
}


unsigned char rfm69_readReg(unsigned char addr) {
  unsigned char thedata[2];
  thedata[0] = addr & 0x7F;
  thedata[1] = 0;

  wiringPiSPIDataRW(SPI_DEVICE, thedata, 2);
  usleep(5);

  return thedata[1];
}

void rfm69_readAllRegs(void) {
  int i;

  for(i = 1; i <= 0x4F; i++) {
   printf("%X - %X\n\r", i, rfm69_readReg(i));
  }
}

void rfm69_setMode(char newMode) {

  if (newMode == _mode) return;
  switch (newMode) {
    case RF69_MODE_TX:
      rfm69_writeReg(REG_OPMODE, (rfm69_readReg(REG_OPMODE) & 0xE3) | RF_OPMODE_TRANSMITTER);
      break;
    case RF69_MODE_RX:
      rfm69_writeReg(REG_OPMODE, (rfm69_readReg(REG_OPMODE) & 0xE3) | RF_OPMODE_RECEIVER);
      break;
    case RF69_MODE_SYNTH:
      rfm69_writeReg(REG_OPMODE, (rfm69_readReg(REG_OPMODE) & 0xE3) | RF_OPMODE_SYNTHESIZER);
      break;
    case RF69_MODE_STANDBY:
      rfm69_writeReg(REG_OPMODE, (rfm69_readReg(REG_OPMODE) & 0xE3) | RF_OPMODE_STANDBY);
      break;
    case RF69_MODE_SLEEP:
      rfm69_writeReg(REG_OPMODE, (rfm69_readReg(REG_OPMODE) & 0xE3) | RF_OPMODE_SLEEP);
      break;
    default: return;
  }

  // we are using packet mode, so this check is not really needed
  // but waiting for mode ready is necessary when going from sleep because the FIFO may not be immediately available from previous mode
  while (_mode == RF69_MODE_SLEEP && (rfm69_readReg(REG_IRQFLAGS1) & RF_IRQFLAGS1_MODEREADY) == 0x00); // Wait for ModeReady
  _mode = newMode;

}

bool rfm69_receive(void) {
    if ((rfm69_readReg(REG_IRQFLAGS2) & RF_IRQFLAGS2_PAYLOADREADY) != 0) {
	  int i = 0;
      for (i = 0; i < PAYLOADSIZE; i++) {
         unsigned char bt = rfm69_readReg(0x00);
        DATA[i] = bt;
      }
      return true;
    }
    return false;
}

void rfm69_getData(unsigned char *data) {
  uint i;
  for(i = 0; i < PAYLOADSIZE; i++) {
    data[i] = DATA[i];
  }
}
