#include "RFM_SENSOR.hpp"
#include "rfm69registers.h"
#include <wiringPiSPI.h>
#define SPI_SPEED 2000000
#define SPI_DEVICE 0

RFM_SENSOR::RFM_SENSOR(void) {
}

RFM_SENSOR::~RFM_SENSOR(void) {
  if(spi != nullptr) {
	  delete spi;
  }
}

int RFM_SENSOR::initializeSensor(unsigned long freq, unsigned long  data_rate) {

  if(spi != nullptr) {
    printf("SPI device already initialized\n\r");
    exit(1);
  }
  
  spi = new mraa::Spi(SPI_DEVICE);
  
  if(spi == nullptr) {
    printf("Error in SPI device initialization\n\r");
    exit(1);
  }
  
  if(spi->mode(mraa::SPI_MODE0) != mraa::SUCCESS || spi->frequency(SPI_SPEED) != mraa::SUCCESS || spi->bitPerWord(8) != mraa::SUCCESS || spi->lsbmode(0) != mraa::SUCCESS) {
    printf("Error in setting SPI device parameters\n\r");
    exit(1);
  }
   
  if(!isRFM69()) {
	  printf("SPI device is no RFM69\n\r");
	  exit(2);
  }

  do writeReg(REG_SYNCVALUE1, 0xaa); while(readReg(REG_SYNCVALUE1) != 0xaa);
  do writeReg(REG_SYNCVALUE1, 0x55); while(readReg(REG_SYNCVALUE1) != 0x55);

  setMode(RF69_MODE_STANDBY);

    /* 0x01 */ writeReg(REG_OPMODE, RF_OPMODE_SEQUENCER_ON | RF_OPMODE_LISTEN_OFF | RF_OPMODE_STANDBY);
    /* 0x02 */ writeReg(REG_DATAMODUL, RF_DATAMODUL_DATAMODE_PACKET | RF_DATAMODUL_MODULATIONTYPE_FSK | RF_DATAMODUL_MODULATIONSHAPING_00);
    /* 0x05 */ writeReg(REG_FDEVMSB, RF_FDEVMSB_90000);
    /* 0x06 */ writeReg(REG_FDEVLSB, RF_FDEVLSB_90000);
    /* 0x11 */ writeReg(REG_PALEVEL, RF_PALEVEL_PA0_ON | RF_PALEVEL_PA1_OFF | RF_PALEVEL_PA2_OFF | RF_PALEVEL_OUTPUTPOWER_11111);
    /* 0x13 */ writeReg(REG_OCP, RF_OCP_OFF);
    /* 0x19 */ writeReg(REG_RXBW, RF_RXBW_DCCFREQ_010 | RF_RXBW_MANT_16 | RF_RXBW_EXP_2);
    /* 0x28 */ writeReg(REG_IRQFLAGS2, RF_IRQFLAGS2_FIFOOVERRUN);
    /* 0x29 */ writeReg(REG_RSSITHRESH, 220);
    /* 0x2E */ writeReg(REG_SYNCCONFIG, RF_SYNC_ON | RF_SYNC_FIFOFILL_AUTO | RF_SYNC_SIZE_2 | RF_SYNC_TOL_0);
    /* 0x2F */ writeReg(REG_SYNCVALUE1, 0x2D);
    /* 0x30 */ writeReg(REG_SYNCVALUE2, 0xD4);
    /* 0x37 */ writeReg(REG_PACKETCONFIG1, RF_PACKET1_CRCAUTOCLEAR_OFF);
    /* 0x38 */ writeReg(REG_PAYLOADLENGTH, PAYLOADSIZE);
    /* 0x3C */ writeReg(REG_FIFOTHRESH, RF_FIFOTHRESH_TXSTART_FIFONOTEMPTY | RF_FIFOTHRESH_VALUE);
    /* 0x3D */ writeReg(REG_PACKETCONFIG2, RF_PACKET2_RXRESTARTDELAY_2BITS | RF_PACKET2_AUTORXRESTART_ON | RF_PACKET2_AES_OFF);
    /* 0x6F */ writeReg(REG_TESTDAGC, RF_DAGC_IMPROVED_LOWBETA0);

  setFrequency(freq);
  setDataRate(data_rate);

  clearFifo();
  
  return 0;
}

int RFM_SENSOR::isRFM69()
{
	  // Is there a RFM69 ?
  writeReg(REG_PAYLOADLENGTH, 0xA);
  if (readReg(REG_PAYLOADLENGTH) == 0xA) {
    writeReg(REG_PAYLOADLENGTH, 0x40);
    if (readReg(REG_PAYLOADLENGTH) == 0x40) {
      return 1;
    }
  }
  return 0;
}

void RFM_SENSOR::writeReg(unsigned char addr, unsigned char value) {
  if(spi != nullptr) {
	unsigned char thedata[2];
	thedata[0] = addr | 0x80;
	thedata[1] = value;

	if(spi->transfer(thedata, nullptr, 2) != mraa::SUCCESS) {
	  printf("Failiure in write on SPI\n\r");
	  exit(4);
	}
	usleep(5);
  } else {
	  printf("Trying to write on not initiliazed SPI\n\r");
	  exit(3);
  }
}

unsigned char RFM_SENSOR::readReg(unsigned char addr) {
  if(spi != nullptr) {
	  unsigned char thedata[2];
	thedata[0] = addr & 0x7F;
	thedata[1] = 0;

	if(spi->transfer(thedata, thedata, 2) != mraa::SUCCESS) {
	  printf("Failiure in read from SPI\n\r");
	  exit(7);
	}
	usleep(5);

	return thedata[1];
  } else {
	  printf("Trying to read from not initiliazed SPI\n\r");
	  exit(6);
  }
}

void RFM_SENSOR::setMode(char newMode) {

  if (newMode == _mode) return;
  switch (newMode) {
    case RF69_MODE_TX:
      writeReg(REG_OPMODE, (readReg(REG_OPMODE) & 0xE3) | RF_OPMODE_TRANSMITTER);
      break;
    case RF69_MODE_RX:
      writeReg(REG_OPMODE, (readReg(REG_OPMODE) & 0xE3) | RF_OPMODE_RECEIVER);
      break;
    case RF69_MODE_SYNTH:
      writeReg(REG_OPMODE, (readReg(REG_OPMODE) & 0xE3) | RF_OPMODE_SYNTHESIZER);
      break;
    case RF69_MODE_STANDBY:
      writeReg(REG_OPMODE, (readReg(REG_OPMODE) & 0xE3) | RF_OPMODE_STANDBY);
      break;
    case RF69_MODE_SLEEP:
      writeReg(REG_OPMODE, (readReg(REG_OPMODE) & 0xE3) | RF_OPMODE_SLEEP);
      break;
    default: return;
  }

  // we are using packet mode, so this check is not really needed
  // but waiting for mode ready is necessary when going from sleep because the FIFO may not be immediately available from previous mode
  while (_mode == RF69_MODE_SLEEP && (readReg(REG_IRQFLAGS1) & RF_IRQFLAGS1_MODEREADY) == 0x00); // Wait for ModeReady
  _mode = newMode;

}

void RFM_SENSOR::clearFifo() {
  writeReg(REG_IRQFLAGS2, 16);
}

void RFM_SENSOR::setDataRate(unsigned long dataRate) {
  m_dataRate = dataRate;

  int r = ((32000000UL + (m_dataRate / 2)) / m_dataRate);
  writeReg(REG_BITRATEMSB, r >> 8);
  if(readReg(REG_BITRATEMSB) != r >> 8) {
	  printf("Error in write REG_BITRATEMSB\n\r");
	  exit(8);
  }
  usleep(100000);
  writeReg(REG_BITRATELSB, r & 0xFF);
  if(readReg(REG_BITRATELSB) != (r & 0xFF)) {
	  printf("Error in write REG_BITRATELSB value: %d\n\r", r & 0xFF);
	  exit(9);
  }
}

unsigned long RFM_SENSOR::getDataRate() {
	return m_dataRate;
}

void RFM_SENSOR::setFrequency(unsigned long kHz) {
  m_frequency = kHz;
  unsigned long f = (((kHz * 1000) << 2) / (32000000L >> 11)) << 6;
  writeReg(REG_FRFMSB, f >> 16);
  writeReg(REG_FRFMID, f >> 8);
  writeReg(REG_FRFLSB, f);
}

bool RFM_SENSOR::receiveData(void) {
    if ((readReg(REG_IRQFLAGS2) & RF_IRQFLAGS2_PAYLOADREADY) != 0) {
	  int i = 0;
      for (i = 0; i < PAYLOADSIZE; i++) {
         unsigned char bt = readReg(0x00);
        DATA[i] = bt;
      }
      return true;
    }
    return false;
}

void RFM_SENSOR::getData(unsigned char *data) {
  uint i;
  for(i = 0; i < PAYLOADSIZE; i++) {
    data[i] = DATA[i];
  }
}

void RFM_SENSOR::printAllRegs(void) {
  int i;

  for(i = 1; i <= 0x4F; i++) {
   printf("%X - %X\n\r", i, readReg(i));
  }
}
