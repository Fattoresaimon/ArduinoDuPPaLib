//
//    FILE: i2cEncoderMiniLib.h
// VERSION: 0.1..
// PURPOSE: Library for I2C Encoder Mini board with Arduino
// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
//
// DATASHEET: https://github.com/Fattoresaimon/I2CEncoderMini
//
//     URL:
//
// AUTHOR:
// Simone Caron
//

#include "i2cEncoderMiniLib.h"
#include <Wire.h>

/*********************************** Public functions *************************************/
/** Class costructor **/
i2cEncoderMiniLib::i2cEncoderMiniLib(uint8_t add) {
  address = add;
}

/** Used for initialize the I2C Encoder Mini **/
void i2cEncoderMiniLib::begin(uint8_t conf) {

  writeEncoder(REG_GCONF, (uint8_t) conf);
  _gconf = conf;
}

/** Used for reset the I2C Encoder Mini **/
void i2cEncoderMiniLib::reset(void) {
  writeEncoder(REG_GCONF, (uint8_t) 0x80);
  delay(10);
}

/** Call che attached callaback if it is defined. It's a prive function only **/
void i2cEncoderMiniLib::eventCaller(Callback *event) {
  if (*event != NULL)
    (*event)(this);
}

/** Return true if the status of the econder changed, otherwise return false.
  It's also call the callback, if attached **/
bool i2cEncoderMiniLib::updateStatus(void) {

  _stat = readEncoderByte(REG_ESTATUS);
  if (_stat == 0) {
    return false;
  }

  if (_stat & PUSHR) {
    eventCaller (&onButtonRelease);
  }
  if (_stat & PUSHP) {
    eventCaller (&onButtonPush);
  }
  if (_stat & PUSHL) {
    eventCaller (&onButtonLongPush);
  }
  if (_stat & PUSHD) {
    eventCaller (&onButtonDoublePush);
  }
  if (_stat & RINC) {
    eventCaller (&onIncrement);
    eventCaller (&onChange);
  }
  if (_stat & RDEC) {
    eventCaller (&onDecrement);
    eventCaller (&onChange);
  }
  if (_stat & RMAX) {
    eventCaller (&onMax);
    eventCaller (&onMinMax);
  }
  if (_stat & RMIN) {
    eventCaller (&onMin);
    eventCaller (&onMinMax);
  }

  return true;
}

/*********************************** Read functions *************************************/

/** Return the INT pin configuration**/
uint8_t i2cEncoderMiniLib::readInterruptConfig(void) {
  return (readEncoderByte(REG_INTCONF));
}

/** Check if a particular status match, return true is match otherwise false. Before require updateStatus() **/
bool i2cEncoderMiniLib::readStatus(Int_Status s) {
  if ((_stat & s) != 0) {
    return true;
  }
  return false;
}

/** Return the status of the encoder **/
uint8_t i2cEncoderMiniLib::readStatus(void) {
  return _stat;
}

/** Return the 32 bit value of the encoder counter  **/
int32_t i2cEncoderMiniLib::readCounterLong(void) {
  return ((int32_t) readEncoderLong(REG_CVALB4));
}

/** Return the 16 bit value of the encoder counter  **/
int16_t i2cEncoderMiniLib::readCounterInt(void) {
  return ((int16_t) readEncoderInt(REG_CVALB2));
}

/** Return the 8 bit value of the encoder counter  **/
int8_t i2cEncoderMiniLib::readCounterByte(void) {
  return ((int8_t) readEncoderByte(REG_CVALB1));
}

/** Return the Maximum threshold of the counter **/
int32_t i2cEncoderMiniLib::readMax(void) {
  return ((int32_t) readEncoderLong(REG_CMAXB4));
}

/** Return the Minimum threshold of the counter **/
int32_t i2cEncoderMiniLib::readMin(void) {
  return ((int32_t) readEncoderLong(REG_CMINB4));
}

/** Return the Steps increment **/
int32_t i2cEncoderMiniLib::readStep(void) {
  return (readEncoderInt(REG_ISTEPB4));
}


/** Read Double push period register **/
uint8_t i2cEncoderMiniLib::readDoublePushPeriod(void) {
  return (readEncoderByte(REG_DPPERIOD));
}

/** Read the ID code **/
uint8_t i2cEncoderMiniLib::readIDCode(void) {
  return (readEncoderByte(REG_IDCODE));
}

/** Read the Version code **/
uint8_t i2cEncoderMiniLib::readVersion(void) {
  return (readEncoderByte(REG_VERSION));
}

/** Read the EEPROM memory**/
uint8_t i2cEncoderMiniLib::readEEPROM(uint8_t add) {

  return (readEncoderByte(add));
}

/*********************************** Write functions *************************************/

/** Write the interrupt configuration **/
void i2cEncoderMiniLib::writeInterruptConfig(uint8_t interrupt) {
  writeEncoder(REG_INTCONF, (uint8_t) interrupt);
}

/** Check if there is some attached callback and enable the corresponding interrupt **/
void i2cEncoderMiniLib::autoconfigInterrupt(void) {
  uint8_t reg=0;

  if (onButtonRelease != NULL)
    reg |= PUSHR;

  if (onButtonPush != NULL)
    reg |= PUSHP;

  if (onButtonDoublePush != NULL)
    reg |= PUSHD;

  if (onButtonLongPush != NULL)
    reg |= PUSHL;

  if (onIncrement != NULL)
    reg |= RINC;

  if (onDecrement != NULL)
    reg |= RDEC;

  if (onChange != NULL) {
    reg |= RINC;
    reg |= RDEC;
  }

  if (onMax != NULL)
    reg |= RMAX;

  if (onMin != NULL)
    reg |= RMIN;

  if (onMinMax != NULL) {
    reg |= RMAX;
    reg |= RMIN;
  }

  writeEncoder(REG_INTCONF, (uint8_t) reg);
}

/** Write the counter value **/
void i2cEncoderMiniLib::writeCounter(int32_t value) {
  writeEncoder(REG_CVALB4, value);
}

/** Write the maximum threshold value **/
void i2cEncoderMiniLib::writeMax(int32_t max) {
  writeEncoder(REG_CMAXB4, max);
}

/** Write the minimum threshold value **/
void i2cEncoderMiniLib::writeMin(int32_t min) {
  writeEncoder(REG_CMINB4, min);
}

/** Write the Step increment value **/
void i2cEncoderMiniLib::writeStep(int32_t step) {
  writeEncoder(REG_ISTEPB4, step);
}

/** Write Double push period register **/
void i2cEncoderMiniLib::writeDoublePushPeriod(uint8_t dperiod) {
  writeEncoder(REG_DPPERIOD, dperiod);
}

/** Change MiniEncoder I2C address **/
void i2cEncoderMiniLib::ChangeI2CAddress(uint8_t add) {
  writeEncoder(REG_I2CADDRESS, add);
  writeEncoder(REG_I2CADDRESS, add);
  writeEncoder(REG_I2CADDRESS, add);
  delay(100);
}

/** Write the EEPROM memory**/
void i2cEncoderMiniLib::writeEEPROM(uint8_t add, uint8_t data) {

  writeEncoder((REG_EEPROMS + add), data);
  delay(5);
}

/*********************************** Private functions *************************************/
/***************************** Read function to the encoder ********************************/

/** Read 1 byte from the encoder **/
uint8_t i2cEncoderMiniLib::readEncoderByte(uint8_t reg) {
  byte rdata = 0xFF;

  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(address, (uint8_t) 1);
  if (Wire.available()) {
    rdata = Wire.read();
  }
  return rdata;
}

/** Read 2 bytes from the encoder **/
int16_t i2cEncoderMiniLib::readEncoderInt(uint8_t reg) {
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(address, (uint8_t) 4);
  if (Wire.available()) {
    _tem_data.bval[1] = Wire.read();
    _tem_data.bval[0] = Wire.read();
  }
  return ((int16_t) _tem_data.val);
}

/** Read 4 bytes from the encoder **/
int32_t i2cEncoderMiniLib::readEncoderLong(uint8_t reg) {

  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(address, (uint8_t) 4);
  if (Wire.available()) {
    _tem_data.bval[3] = Wire.read();
    _tem_data.bval[2] = Wire.read();
    _tem_data.bval[1] = Wire.read();
    _tem_data.bval[0] = Wire.read();
  }
  return ((int32_t) _tem_data.val);
}

/***************************** Write function to the encoder ********************************/
/** Send to the encoder 1 byte **/
void i2cEncoderMiniLib::writeEncoder(uint8_t reg, uint8_t data) {

  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}

/** Send to the encoder 4 byte **/
void i2cEncoderMiniLib::writeEncoder(uint8_t reg, int32_t data) {
  uint8_t temp[4];
  _tem_data.val = data;
  temp[0] = _tem_data.bval[3];
  temp[1] = _tem_data.bval[2];
  temp[2] = _tem_data.bval[1];
  temp[3] = _tem_data.bval[0];
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(temp, (uint8_t) 4);
  Wire.endTransmission();
}
