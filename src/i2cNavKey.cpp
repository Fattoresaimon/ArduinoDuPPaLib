//
//    FILE: i2cNavKey.h
// VERSION: 0.1..
// PURPOSE: Library for I2C NavKey V2 board with Arduino
// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
//
// DATASHEET:
//
//     URL:
//
// AUTHOR:
// Simone Caron
//

#include "i2cNavKey.h"
#include <Wire.h>

/*********************************** Public functions *************************************/
/** Class costructor **/
i2cNavKey::i2cNavKey(uint8_t add) {
  _add = add;
}

/** Used for initialize the NavKey **/
void i2cNavKey::begin(uint8_t conf) {


  writeNavKey(REG_GCONF, conf);
  _gconf = conf;

}
void i2cNavKey::reset(void) {
	writeNavKey(REG_GCONF, (uint8_t) 0x80);
	delay(10);
}

/** Call che attached callaback if it is defined. It's a prive function only **/
void i2cNavKey::eventCaller(Callback *event) {
	if (*event != NULL)
		(*event)(this);
}

/** Return true if the status of the econder changed, otherwise return false. 
 It's also call the callback, if attached **/
bool i2cNavKey::updateStatus(void) {

	_stat = readNavKeyInt(REG_STATUSB2);
	
	_stat2 = 0;
	if (_stat == 0) {
		return false;
	}

	if (_stat & UPR) {
		eventCaller (&onUpRelease);
		eventCaller (&onArrowsRelese);
	}
	if (_stat & UPP) {
		eventCaller (&onUpPush);
		eventCaller (&onArrowsPush);
	}
	if (_stat & DNR) {
		eventCaller (&onDownRelease);
		eventCaller (&onArrowsRelese);
	}
	if (_stat & DNP) {
		eventCaller (&onDownPush);
		eventCaller (&onArrowsPush);
	}
	if (_stat & RTR) {
		eventCaller (&onRightRelease);
		eventCaller (&onArrowsRelese);
	}
	if (_stat & RTP) {
		eventCaller (&onRightPush);
		eventCaller (&onArrowsPush);
	}
	if (_stat & LTR) {
		eventCaller (&onLeftRelease);
		eventCaller (&onArrowsRelese);
	}
	if (_stat & LTP) {
		eventCaller (&onLeftPush);
		eventCaller (&onArrowsPush);
	}
	if (_stat & CTRR) {
		eventCaller (&onCentralRelease);
	}
	if (_stat & CTRP) {
		eventCaller (&onCentralPush);
	}
	if (_stat & CTRDP) {
		eventCaller (&onCentralDoublePush);
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
	
	if ((_stat & INT_2) != 0) {
		_stat2 = readNavKeyByte(REG_SSTATUS);
	if (_stat2 == 0) {
			return true;
		}

		if (_stat2 & GP1_POS) {
			eventCaller (&onGP1Rise);
		}
		if (_stat2 & GP1_NEG) {
			eventCaller (&onGP1Fall);
		}
		if (_stat2 & GP2_POS) {
			eventCaller (&onGP2Rise);
		}
		if (_stat2 & GP2_NEG) {
			eventCaller (&onGP2Fall);
		}
		if (_stat2 & GP3_POS) {
			eventCaller (&onGP3Rise);
		}
		if (_stat2 & GP3_NEG) {
			eventCaller (&onGP3Fall);
		}
		if (_stat2 & FADE_INT) {
			eventCaller (&onFadeProcess);
		}
	}

	return true;
}

/*********************************** Read functions *************************************/

/** Return the GP1 Configuration**/
uint8_t i2cNavKey::readGP1conf(void) {
  return (readNavKeyByte(REG_GP1CONF));
}

/** Return the GP1 Configuration**/
uint8_t i2cNavKey::readGP2conf(void) {
  return (readNavKeyByte(REG_GP2CONF));
}

/** Return the GP1 Configuration**/
uint8_t i2cNavKey::readGP3conf(void) {
  return (readNavKeyByte(REG_GP3CONF));
}

/** Return the INT pin configuration**/
uint16_t i2cNavKey::readInterruptConfig(void) {
  return ((uint16_t)readNavKeyInt(REG_INTCONFB2));
}


/** Check if a particular status match, return true is match otherwise false. Before require updateStatus() **/
bool i2cNavKey::readStatus(Int_Status s) {
  if ((_stat & s) != 0) {
    return true;
  }
  return false;
}

/** Return the status of the NavKey **/
uint16_t i2cNavKey::readStatus(void) {
  return _stat;
}


/** Check if a particular status of the Int2 match, return true is match otherwise false. Before require updateStatus() **/
bool i2cNavKey::readInt2(Int2_Status s) {
  if ((_stat2 & s)  != 0) {
    return true;
  }
  return false;
}

/** Return the Int2 status of the NavKey. Before require updateStatus()  **/
uint8_t i2cNavKey::readInt2(void) {
  return _stat2;
}

/** Return Fade process status  **/
uint8_t i2cNavKey::readFadeStatus(void) {
  return readNavKeyByte(REG_FSTATUS);
}

/** Check if a particular status of the Fade process match, return true is match otherwise false. **/
bool i2cNavKey::readFadeStatus(Fade_Status s) {
  if ((readNavKeyByte(REG_FSTATUS) & s) == 1)
    return true;

  return false;
}

/** Return the 32 bit value of the NavKey counter  **/
float i2cNavKey::readCounterFloat(void) {
  return (readNavKeyFloat(REG_CVALB4));
}

/** Return the 32 bit value of the NavKey counter  **/
int32_t i2cNavKey::readCounterLong(void) {
  return ((int32_t) readNavKeyLong(REG_CVALB4));
}

/** Return the 16 bit value of the NavKey counter  **/
int16_t i2cNavKey::readCounterInt(void) {
  return ((int16_t) readNavKeyInt(REG_CVALB2));
}

/** Return the 8 bit value of the NavKey counter  **/
int8_t i2cNavKey::readCounterByte(void) {
  return ((int8_t) readNavKeyByte(REG_CVALB1));
}

/** Return the Maximum threshold of the counter **/
int32_t i2cNavKey::readMax(void) {
  return ((int32_t) readNavKeyLong(REG_CMAXB4));
}

/** Return the Minimum threshold of the counter **/
int32_t i2cNavKey::readMin(void) {
  return ((int32_t) readNavKeyLong(REG_CMINB4));
}

/** Return the Maximum threshold of the counter **/
float i2cNavKey::readMaxFloat(void) {
  return (readNavKeyFloat(REG_CMAXB4));
}

/** Return the Minimum threshold of the counter **/
float i2cNavKey::readMinFloat(void) {
  return (readNavKeyFloat(REG_CMINB4));

}

/** Return the Steps increment **/
int32_t i2cNavKey::readStep(void) {
  return (readNavKeyInt(REG_ISTEPB4));
}

/** Return the Steps increment, in float variable **/
float i2cNavKey::readStepFloat(void) {
  return (readNavKeyFloat(REG_ISTEPB4));

}

/** Read GP1 register value **/
uint8_t i2cNavKey::readGP1(void) {
  return (readNavKeyByte(REG_GP1REG));
}

/** Read GP2 register value **/
uint8_t i2cNavKey::readGP2(void) {
  return (readNavKeyByte(REG_GP2REG));
}

/** Read GP3 register value **/
uint8_t i2cNavKey::readGP3(void) {
  return (readNavKeyByte(REG_GP3REG));
}

/** Read Double push period register **/
uint8_t i2cNavKey::readDoublePushPeriod(void) {
  return (readNavKeyByte(REG_DPPERIOD));
}

/** Read the fade period of the GP LED**/
uint8_t i2cNavKey::readFadeGP(void) {
  return (readNavKeyByte(REG_FADEGP));
}

/** Read the EEPROM memory**/
uint8_t i2cNavKey::readEEPROM(uint8_t add) {
  if (add <= 0x7f) {
    if ((_gconf & EEPROM_BANK1) != 0) {
      _gconf = _gconf & 0xBF;
      writeNavKey(REG_GCONF, _gconf);
    }
    return (readNavKeyByte((REG_EEPROMS + add)));
  } else {
    if ((_gconf & EEPROM_BANK1) == 0) {
      _gconf = _gconf | 0x40;
      writeNavKey(REG_GCONF, _gconf);
    }
    return (readNavKeyByte(add));
  }
}

/** Read the ID code **/
uint8_t i2cNavKey::readIDCode(void) {
  return (readNavKeyByte(REG_IDCODE));
}

/** Read the Version code **/
uint8_t i2cNavKey::readVersion(void) {
  return (readNavKeyByte(REG_VERSION));
}



/*********************************** Write functions *************************************/
/** Write the GP1 configuration**/
void i2cNavKey::writeGP1conf(uint8_t gp1) {
  writeNavKey(REG_GP1CONF, gp1);
}

/** Write the GP2 configuration**/
void i2cNavKey::writeGP2conf(uint8_t gp2) {
  writeNavKey(REG_GP2CONF, gp2);
}

/** Write the GP3 configuration**/
void i2cNavKey::writeGP3conf(uint8_t gp3) {
  writeNavKey(REG_GP3CONF, gp3);
}

/** Write the interrupt configuration **/
void i2cNavKey::writeInterruptConfig(uint16_t interrupt) {
  writeNavKey(REG_INTCONFB2, interrupt);
}

/** Check if there is some attached callback and enable the corresponding interrupt **/
void i2cNavKey::autoconfigInterrupt(void) {
	uint16_t reg=0;

	if (onArrowsPush != NULL)
		reg = reg | UPP | DNP | RTP | LTP;
	

	if (onArrowsRelese != NULL)
		reg = reg | UPR | DNR | RTR | LTR;

	if (onUpPush != NULL)
		reg |= UPP;
	
	if (onUpRelease != NULL)
		reg |= UPR;

	if (onDownPush != NULL)
		reg |= DNP;

	if (onDownRelease != NULL)
		reg |= DNR;
	
	if (onRightPush != NULL)
		reg |= RTP;

	if (onRightRelease != NULL)
		reg |= RTR;

	if (onLeftPush != NULL)
		reg |= LTP;

	if (onLeftRelease != NULL)
		reg |= LTR;
	
	if (onCentralPush != NULL)
		reg |= CTRP;
	
	if (onCentralRelease != NULL)
		reg |= CTRR;
	
	if (onCentralDoublePush != NULL)
		reg |= CTRDP;

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

	if (onGP1Rise != NULL)
		reg |= INT_2;

	if (onGP1Fall != NULL)
		reg |= INT_2;

	if (onGP2Rise != NULL)
		reg |= INT_2;

	if (onGP2Fall != NULL)
		reg |= INT_2;

	if (onGP3Rise != NULL)
		reg |= INT_2;

	if (onGP3Fall != NULL)
		reg |= INT_2;

	if (onFadeProcess != NULL)
		reg |= INT_2;

	writeInterruptConfig((uint16_t) reg);
}



/** Write the counter value **/
void i2cNavKey::writeCounter(int32_t value) {
  writeNavKey(REG_CVALB4, value);
}

/** Write the counter value **/
void i2cNavKey::writeCounter(float value) {
  writeNavKey(REG_CVALB4, value);
}

/** Write the maximum threshold value **/
void i2cNavKey::writeMax(int32_t max) {
  writeNavKey(REG_CMAXB4, max);
}

/** Write the maximum threshold value **/
void i2cNavKey::writeMax(float max) {
  writeNavKey(REG_CMAXB4, max);
}

/** Write the minimum threshold value **/
void i2cNavKey::writeMin(int32_t min) {
  writeNavKey(REG_CMINB4, min);
}

/** Write the minimum threshold value **/
void i2cNavKey::writeMin(float min) {
  writeNavKey(REG_CMINB4, min);
}

/** Write the Step increment value **/
void i2cNavKey::writeStep(int32_t step) {
  writeNavKey(REG_ISTEPB4, step);
}

/** Write the Step increment value **/
void i2cNavKey::writeStep(float step) {
  writeNavKey(REG_ISTEPB4, step);
}

/** Write GP1 register, used when GP1 is set to output or PWM **/
void i2cNavKey::writeGP1(uint8_t gp1) {
  writeNavKey(REG_GP1REG, gp1);
}

/** Write GP2 register, used when GP2 is set to output or PWM **/
void i2cNavKey::writeGP2(uint8_t gp2) {
  writeNavKey(REG_GP2REG, gp2);
}

/** Write GP3 register, used when GP3 is set to output or PWM **/
void i2cNavKey::writeGP3(uint8_t gp3) {
  writeNavKey(REG_GP3REG, gp3);
}

/** Write Anti-bouncing period register **/
void i2cNavKey::writeDoublePushPeriod(uint8_t dperiod) {
  writeNavKey(REG_DPPERIOD, dperiod);
}

/** Write Fade timing in ms **/
void i2cNavKey::writeFadeGP(uint8_t fade) {
  writeNavKey(REG_FADEGP, fade);
}

/** Write the Gamma value on GP1 **/
void i2cNavKey::writeGammaGP1(GAMMA_PARAMETER Gamma) {
  writeNavKey(REG_GAMMAGP1, (uint8_t) Gamma);
}

/** Write the Gamma value on GP2 **/
void i2cNavKey::writeGammaGP2(GAMMA_PARAMETER Gamma) {
  writeNavKey(REG_GAMMAGP2, (uint8_t)Gamma);
}

/** Write the Gamma value on GP1 **/
void i2cNavKey::writeGammaGP3(GAMMA_PARAMETER Gamma) {
  writeNavKey(REG_GAMMAGP3, (uint8_t)Gamma);
}

/** Write the EEPROM memory**/
void i2cNavKey::writeEEPROM(uint8_t add, uint8_t data) {
  if (add <= 0x7f) {
    if ((_gconf & EEPROM_BANK1) != 0) {
      _gconf = _gconf & 0xBF;
      writeNavKey(REG_GCONF, _gconf);
    }
    writeNavKey((REG_EEPROMS + add), data);
  } else {
    if ((_gconf & EEPROM_BANK1) == 0) {
      _gconf = _gconf | 0x40;
      writeNavKey(REG_GCONF, _gconf);
    }
    writeNavKey(add, data);
  }
}

/*********************************** Private functions *************************************/
/***************************** Read function to the NavKey ********************************/

/** Read 1 byte from the NavKey **/
uint8_t i2cNavKey::readNavKeyByte(uint8_t reg) {
  byte rdata = 0xFF;

  Wire.beginTransmission(_add);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom((int)_add, (int)1);
  if (Wire.available()) {
    rdata = Wire.read();
  }
  //	delay(5);
  return rdata;
}

/** Read 2 bytes from the NavKey **/
int16_t i2cNavKey::readNavKeyInt(uint8_t reg) {
  Wire.beginTransmission(_add);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom((int)_add, (int)2);
  if (Wire.available()) {
    _tem_data.bval[1] = Wire.read();
    _tem_data.bval[0] = Wire.read();
  }
  return ((int16_t) _tem_data.wval);
}

/** Read 4 bytes from the NavKey **/
int32_t i2cNavKey::readNavKeyLong(uint8_t reg) {

  Wire.beginTransmission(_add);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom((int)_add, (int)4);
  if (Wire.available()) {
    _tem_data.bval[3] = Wire.read();
    _tem_data.bval[2] = Wire.read();
    _tem_data.bval[1] = Wire.read();
    _tem_data.bval[0] = Wire.read();
  }
  return ((int32_t) _tem_data.val);
}

/** Read 4 bytes from the NavKey **/
float i2cNavKey::readNavKeyFloat(uint8_t reg) {
  Wire.beginTransmission(_add);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom((int)_add, (int)4);
  if (Wire.available()) {
    _tem_data.bval[3] = Wire.read();
    _tem_data.bval[2] = Wire.read();
    _tem_data.bval[1] = Wire.read();
    _tem_data.bval[0] = Wire.read();
  }
  return ((float) _tem_data.fval);
}

/***************************** Write function to the NavKey ********************************/
/** Send to the NavKey 1 byte **/
void i2cNavKey::writeNavKey(uint8_t reg, uint8_t data) {

  Wire.beginTransmission(_add);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
  //  delay(1);

}

/** Send to the NavKey 4 byte **/
void i2cNavKey::writeNavKey(uint8_t reg, int32_t data) {
  uint8_t temp[4];
  _tem_data.val = data;
  temp[0] = _tem_data.bval[3];
  temp[1] = _tem_data.bval[2];
  temp[2] = _tem_data.bval[1];
  temp[3] = _tem_data.bval[0];
  Wire.beginTransmission(_add);
  Wire.write(reg);
  Wire.write(temp, 4);
  Wire.endTransmission();
  // delay(1);
}


/** Send to the NavKey 2 byte **/
void i2cNavKey::writeNavKey(uint8_t reg, uint16_t data) {
  uint8_t temp[2];
  _tem_data.wval = data;
  temp[0] = _tem_data.bval[1];
  temp[1] = _tem_data.bval[0];
  Wire.beginTransmission(_add);
  Wire.write(reg);
  Wire.write(temp, 2);
  Wire.endTransmission();
  // delay(1);
}

/** Send to the NavKey 4 byte for floating number **/
void i2cNavKey::writeNavKey(uint8_t reg, float data) {

  uint8_t temp[4];
  _tem_data.fval = data;
  temp[0] = _tem_data.bval[3];
  temp[1] = _tem_data.bval[2];
  temp[2] = _tem_data.bval[1];
  temp[3] = _tem_data.bval[0];
  Wire.beginTransmission(_add);
  Wire.write(reg);
  Wire.write(temp, 4);
  Wire.endTransmission();
  //  delay(1);

}


/** Send to the NavKey 3 byte **/
void i2cNavKey::writeNavKey24bit(uint8_t reg, uint32_t data) {
  uint8_t temp[3];
  _tem_data.val = data;
  temp[0] = _tem_data.bval[2];
  temp[1] = _tem_data.bval[1];
  temp[2] = _tem_data.bval[0];
  Wire.beginTransmission(_add);
  Wire.write(reg);
  Wire.write(temp, 3);
  Wire.endTransmission();
  // delay(1);

}
