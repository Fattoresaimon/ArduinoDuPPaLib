//
//    FILE: i2cEncoderLibV2.h
// VERSION: 1.0..
// PURPOSE: Library for I2C Encoder V2.1 board with Arduino
// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
//
// DATASHEET:
//
//     URL:
//
// AUTHOR:
// Simone Caron
//

#include "i2cEncoderLibV2.h"
#include <Wire.h>

/*********************************** Public functions *************************************/
/** Class costructor **/
i2cEncoderLibV2::i2cEncoderLibV2(uint8_t add) {
	_add = add;
}

/** Used for initialize the encoder **/
void i2cEncoderLibV2::begin(uint16_t conf) {

	writeEncoder(REG_GCONF, (uint8_t)( conf & 0xFF));
	writeEncoder(REG_GCONF2, (uint8_t)((conf >> 8) & 0xFF));
	_gconf = conf;
	if ((conf & CLK_STRECH_ENABLE) == 0)
		_clockstreach = 0;
	else
		_clockstreach = 1;
}

/** Reset the board **/
void i2cEncoderLibV2::reset(void) {
	writeEncoder(REG_GCONF, (uint8_t) 0x80);
	delay(10);
}

/** Call che attached callaback if it is defined. It's a prive function only **/
void i2cEncoderLibV2::eventCaller(Callback *event) {
	if (*event != NULL)
		(*event)(this);
}

/** Return true if the status of the econder changed, otherwise return false. 
 It's also call the callback, if attached **/
bool i2cEncoderLibV2::updateStatus(void) {

	_stat = readEncoderByte(REG_ESTATUS);
	_stat2 = 0;
	if (_stat == 0) {
		return false;
	}

	if (_stat & PUSHR) {
		eventCaller (&onButtonRelease);
	}
	if (_stat & PUSHP) {
		eventCaller (&onButtonPush);
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

	if ((_stat & INT_2) != 0) {
		_stat2 = readEncoderByte(REG_I2STATUS);
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
uint8_t i2cEncoderLibV2::readGP1conf(void) {
	return (readEncoderByte(REG_GP1CONF));
}

/** Return the GP1 Configuration**/
uint8_t i2cEncoderLibV2::readGP2conf(void) {
	return (readEncoderByte(REG_GP2CONF));
}

/** Return the GP1 Configuration**/
uint8_t i2cEncoderLibV2::readGP3conf(void) {
	return (readEncoderByte(REG_GP3CONF));
}

/** Return the INT pin configuration**/
uint8_t i2cEncoderLibV2::readInterruptConfig(void) {
	return (readEncoderByte(REG_INTCONF));
}

/** Check if a particular status match, return true is match otherwise false. Before require updateStatus() **/
bool i2cEncoderLibV2::readStatus(Int_Status s) {
	if ((_stat & s) != 0) {
		return true;
	}
	return false;
}

/** Return the status of the encoder **/
uint8_t i2cEncoderLibV2::readStatus(void) {
	return _stat;
}

/** Check if a particular status of the Int2 match, return true is match otherwise false. Before require updateStatus() **/
bool i2cEncoderLibV2::readInt2(Int2_Status s) {
	if ((_stat2 & s) != 0) {
		return true;
	}
	return false;
}

/** Return the Int2 status of the encoder. Before require updateStatus()  **/
uint8_t i2cEncoderLibV2::readInt2(void) {
	return _stat2;
}

/** Return Fade process status  **/
uint8_t i2cEncoderLibV2::readFadeStatus(void) {
	return readEncoderByte(REG_FSTATUS);
}

/** Check if a particular status of the Fade process match, return true is match otherwise false. **/
bool i2cEncoderLibV2::readFadeStatus(Fade_Status s) {
	if ((readEncoderByte(REG_FSTATUS) & s) == 1)
		return true;

	return false;
}

/** Return the PWM LED R value  **/
uint8_t i2cEncoderLibV2::readLEDR(void) {
	return ((uint8_t) readEncoderByte(REG_RLED));
}

/** Return the PWM LED G value  **/
uint8_t i2cEncoderLibV2::readLEDG(void) {
	return ((uint8_t) readEncoderByte(REG_GLED));
}

/** Return the PWM LED B value  **/
uint8_t i2cEncoderLibV2::readLEDB(void) {
	return ((uint8_t) readEncoderByte(REG_BLED));
}

/** Return the 32 bit value of the encoder counter  **/
float i2cEncoderLibV2::readCounterFloat(void) {
	return (readEncoderFloat(REG_CVALB4));
}

/** Return the 32 bit value of the encoder counter  **/
int32_t i2cEncoderLibV2::readCounterLong(void) {
	return ((int32_t) readEncoderLong(REG_CVALB4));
}

/** Return the 16 bit value of the encoder counter  **/
int16_t i2cEncoderLibV2::readCounterInt(void) {
	return ((int16_t) readEncoderInt(REG_CVALB2));
}

/** Return the 8 bit value of the encoder counter  **/
int8_t i2cEncoderLibV2::readCounterByte(void) {
	return ((int8_t) readEncoderByte(REG_CVALB1));
}

/** Return the Maximum threshold of the counter **/
int32_t i2cEncoderLibV2::readMax(void) {
	return ((int32_t) readEncoderLong(REG_CMAXB4));
}

/** Return the Minimum threshold of the counter **/
int32_t i2cEncoderLibV2::readMin(void) {
	return ((int32_t) readEncoderLong(REG_CMINB4));
}

/** Return the Maximum threshold of the counter **/
float i2cEncoderLibV2::readMaxFloat(void) {
	return (readEncoderFloat(REG_CMAXB4));
}

/** Return the Minimum threshold of the counter **/
float i2cEncoderLibV2::readMinFloat(void) {
	return (readEncoderFloat(REG_CMINB4));

}

/** Return the Steps increment **/
int32_t i2cEncoderLibV2::readStep(void) {
	return (readEncoderInt(REG_ISTEPB4));
}

/** Return the Steps increment, in float variable **/
float i2cEncoderLibV2::readStepFloat(void) {
	return (readEncoderFloat(REG_ISTEPB4));

}

/** Read GP1 register value **/
uint8_t i2cEncoderLibV2::readGP1(void) {
	return (readEncoderByte(REG_GP1REG));
}

/** Read GP2 register value **/
uint8_t i2cEncoderLibV2::readGP2(void) {
	return (readEncoderByte(REG_GP2REG));
}

/** Read GP3 register value **/
uint8_t i2cEncoderLibV2::readGP3(void) {
	return (readEncoderByte(REG_GP3REG));
}

/** Read Anti-bouncing period register **/
uint8_t i2cEncoderLibV2::readAntibouncingPeriod(void) {
	return (readEncoderByte(REG_ANTBOUNC));
}

/** Read Double push period register **/
uint8_t i2cEncoderLibV2::readDoublePushPeriod(void) {
	return (readEncoderByte(REG_DPPERIOD));
}

/** Read the fade period of the RGB LED**/
uint8_t i2cEncoderLibV2::readFadeRGB(void) {
	return (readEncoderByte(REG_FADERGB));
}

/** Read the fade period of the GP LED**/
uint8_t i2cEncoderLibV2::readFadeGP(void) {
	return (readEncoderByte(REG_FADEGP));
}

/** Read the ID code **/
uint8_t i2cEncoderLibV2::readIDCode(void) {
	return (readEncoderByte(REG_IDCODE));
}

/** Read the Version code **/
uint8_t i2cEncoderLibV2::readVersion(void) {
	return (readEncoderByte(REG_VERSION));
}

/** Read the EEPROM memory**/
uint8_t i2cEncoderLibV2::readEEPROM(uint8_t add) {
	if (add <= 0x7f) {
		if ((_gconf & EEPROM_BANK1) != 0) {
			_gconf = _gconf & 0xBF;
			writeEncoder(REG_GCONF, _gconf);
		}
		return (readEncoderByte((REG_EEPROMS + add)));
	} else {
		if ((_gconf & EEPROM_BANK1) == 0) {
			_gconf = _gconf | 0x40;
			writeEncoder(REG_GCONF, _gconf);
		}
		return (readEncoderByte(add));
	}
}

/*********************************** Write functions *************************************/
/** Write the GP1 configuration**/
void i2cEncoderLibV2::writeGP1conf(uint8_t gp1) {
	writeEncoder(REG_GP1CONF, (uint8_t) gp1);
}

/** Write the GP2 configuration**/
void i2cEncoderLibV2::writeGP2conf(uint8_t gp2) {
	writeEncoder(REG_GP2CONF, (uint8_t) gp2);
}

/** Write the GP3 configuration**/
void i2cEncoderLibV2::writeGP3conf(uint8_t gp3) {
	writeEncoder(REG_GP3CONF, (uint8_t) gp3);
}

/** Write the interrupt configuration **/
void i2cEncoderLibV2::writeInterruptConfig(uint8_t interrupt) {
	writeEncoder(REG_INTCONF, (uint8_t) interrupt);
}

/** Check if there is some attached callback and enable the corresponding interrupt **/
void i2cEncoderLibV2::autoconfigInterrupt(void) {
	uint8_t reg=0;

	if (onButtonRelease != NULL)
		reg |= PUSHR;

	if (onButtonPush != NULL)
		reg |= PUSHP;

	if (onButtonDoublePush != NULL)
		reg |= PUSHD;

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

	writeEncoder(REG_INTCONF, (uint8_t) reg);
}

/** Write the counter value **/
void i2cEncoderLibV2::writeCounter(int32_t value) {
	writeEncoder(REG_CVALB4, value);
}

/** Write the counter value **/
void i2cEncoderLibV2::writeCounter(float value) {
	writeEncoder(REG_CVALB4, value);
}

/** Write the maximum threshold value **/
void i2cEncoderLibV2::writeMax(int32_t max) {
	writeEncoder(REG_CMAXB4, max);
}

/** Write the maximum threshold value **/
void i2cEncoderLibV2::writeMax(float max) {
	writeEncoder(REG_CMAXB4, max);
}

/** Write the minimum threshold value **/
void i2cEncoderLibV2::writeMin(int32_t min) {
	writeEncoder(REG_CMINB4, min);
}

/** Write the minimum threshold value **/
void i2cEncoderLibV2::writeMin(float min) {
	writeEncoder(REG_CMINB4, min);
}

/** Write the Step increment value **/
void i2cEncoderLibV2::writeStep(int32_t step) {
	writeEncoder(REG_ISTEPB4, step);
}

/** Write the Step increment value **/
void i2cEncoderLibV2::writeStep(float step) {
	writeEncoder(REG_ISTEPB4, step);
}

/** Write the PWM value of the RGB LED red **/
void i2cEncoderLibV2::writeLEDR(uint8_t rled) {
	writeEncoder(REG_RLED, rled);
}

/** Write the PWM value of the RGB LED green **/
void i2cEncoderLibV2::writeLEDG(uint8_t gled) {
	writeEncoder(REG_GLED, gled);
}

/** Write the PWM value of the RGB LED blue **/
void i2cEncoderLibV2::writeLEDB(uint8_t bled) {
	writeEncoder(REG_BLED, bled);
}

/** Write 24bit color code **/
void i2cEncoderLibV2::writeRGBCode(uint32_t rgb) {
	writeEncoder24bit(REG_RLED, rgb);
}

/** Write GP1 register, used when GP1 is set to output or PWM **/
void i2cEncoderLibV2::writeGP1(uint8_t gp1) {
	writeEncoder(REG_GP1REG, gp1);
}

/** Write GP2 register, used when GP2 is set to output or PWM **/
void i2cEncoderLibV2::writeGP2(uint8_t gp2) {
	writeEncoder(REG_GP2REG, gp2);
}

/** Write GP3 register, used when GP3 is set to output or PWM **/
void i2cEncoderLibV2::writeGP3(uint8_t gp3) {
	writeEncoder(REG_GP3REG, gp3);
}

/** Write Anti-bouncing period register **/
void i2cEncoderLibV2::writeAntibouncingPeriod(uint8_t bounc) {
	writeEncoder(REG_ANTBOUNC, bounc);
}

/** Write Anti-bouncing period register **/
void i2cEncoderLibV2::writeDoublePushPeriod(uint8_t dperiod) {
	writeEncoder(REG_DPPERIOD, dperiod);
}

/** Write Fade timing in ms **/
void i2cEncoderLibV2::writeFadeRGB(uint8_t fade) {
	writeEncoder(REG_FADERGB, fade);
}

/** Write Fade timing in ms **/
void i2cEncoderLibV2::writeFadeGP(uint8_t fade) {
	writeEncoder(REG_FADEGP, fade);
}

/** Write the Gamma value on RLED **/
void i2cEncoderLibV2::writeGammaRLED(GAMMA_PARAMETER Gamma) {
	writeEncoder(REG_GAMRLED, (uint8_t) Gamma);
}

/** Write the Gamma value on GLED **/
void i2cEncoderLibV2::writeGammaGLED(GAMMA_PARAMETER Gamma) {
	writeEncoder(REG_GAMGLED, (uint8_t) Gamma);
}

/** Write the Gamma value on BLED **/
void i2cEncoderLibV2::writeGammaBLED(GAMMA_PARAMETER Gamma) {
	writeEncoder(REG_GAMBLED, (uint8_t) Gamma);
}

/** Write the Gamma value on GP1 **/
void i2cEncoderLibV2::writeGammaGP1(GAMMA_PARAMETER Gamma) {
	writeEncoder(REG_GAMMAGP1, (uint8_t) Gamma);
}

/** Write the Gamma value on GP2 **/
void i2cEncoderLibV2::writeGammaGP2(GAMMA_PARAMETER Gamma) {
	writeEncoder(REG_GAMMAGP2, (uint8_t) Gamma);
}

/** Write the Gamma value on GP1 **/
void i2cEncoderLibV2::writeGammaGP3(GAMMA_PARAMETER Gamma) {
	writeEncoder(REG_GAMMAGP3, (uint8_t) Gamma);
}

/** Write the EEPROM memory**/
void i2cEncoderLibV2::writeEEPROM(uint8_t add, uint8_t data) {

	if (add <= 0x7f) {
		if ((_gconf & EEPROM_BANK1) != 0) {
			_gconf = _gconf & 0xBF;
			writeEncoder(REG_GCONF, _gconf);
		}
		writeEncoder((REG_EEPROMS + add), data);
	} else {
		if ((_gconf & EEPROM_BANK1) == 0) {
			_gconf = _gconf | 0x40;
			writeEncoder(REG_GCONF, _gconf);
		}
		writeEncoder(add, data);
	}

	if (_clockstreach == 0)
		delay(5);
}

/*********************************** Private functions *************************************/
/***************************** Read function to the encoder ********************************/

/** Read 1 byte from the encoder **/
uint8_t i2cEncoderLibV2::readEncoderByte(uint8_t reg) {
	byte rdata = 0xFF;

	Wire.beginTransmission(_add);
	Wire.write(reg);
	Wire.endTransmission();
	Wire.requestFrom(_add, (uint8_t) 1);
	if (Wire.available()) {
		rdata = Wire.read();
	}
	return rdata;
}

/** Read 2 bytes from the encoder **/
int16_t i2cEncoderLibV2::readEncoderInt(uint8_t reg) {
	Wire.beginTransmission(_add);
	Wire.write(reg);
	Wire.endTransmission();
	Wire.requestFrom(_add, (uint8_t) 4);
	if (Wire.available()) {
		_tem_data.bval[1] = Wire.read();
		_tem_data.bval[0] = Wire.read();
	}
	return ((int16_t) _tem_data.val);
}

/** Read 4 bytes from the encoder **/
int32_t i2cEncoderLibV2::readEncoderLong(uint8_t reg) {

	Wire.beginTransmission(_add);
	Wire.write(reg);
	Wire.endTransmission();
	Wire.requestFrom(_add, (uint8_t) 4);
	if (Wire.available()) {
		_tem_data.bval[3] = Wire.read();
		_tem_data.bval[2] = Wire.read();
		_tem_data.bval[1] = Wire.read();
		_tem_data.bval[0] = Wire.read();
	}
	return ((int32_t) _tem_data.val);
}

/** Read 4 bytes from the encoder **/
float i2cEncoderLibV2::readEncoderFloat(uint8_t reg) {
	Wire.beginTransmission(_add);
	Wire.write(reg);
	Wire.endTransmission();
	Wire.requestFrom(_add, (uint8_t) 4);
	if (Wire.available()) {
		_tem_data.bval[3] = Wire.read();
		_tem_data.bval[2] = Wire.read();
		_tem_data.bval[1] = Wire.read();
		_tem_data.bval[0] = Wire.read();
	}
	return ((float) _tem_data.fval);
}

/***************************** Write function to the encoder ********************************/
/** Send to the encoder 1 byte **/
void i2cEncoderLibV2::writeEncoder(uint8_t reg, uint8_t data) {

	Wire.beginTransmission(_add);
	Wire.write(reg);
	Wire.write(data);
	Wire.endTransmission();
}

/** Send to the encoder 4 byte **/
void i2cEncoderLibV2::writeEncoder(uint8_t reg, int32_t data) {
	uint8_t temp[4];
	_tem_data.val = data;
	temp[0] = _tem_data.bval[3];
	temp[1] = _tem_data.bval[2];
	temp[2] = _tem_data.bval[1];
	temp[3] = _tem_data.bval[0];
	Wire.beginTransmission(_add);
	Wire.write(reg);
	Wire.write(temp, (uint8_t) 4);
	Wire.endTransmission();
}

/** Send to the encoder 4 byte for floating number **/
void i2cEncoderLibV2::writeEncoder(uint8_t reg, float data) {

	uint8_t temp[4];
	_tem_data.fval = data;
	temp[0] = _tem_data.bval[3];
	temp[1] = _tem_data.bval[2];
	temp[2] = _tem_data.bval[1];
	temp[3] = _tem_data.bval[0];
	Wire.beginTransmission(_add);
	Wire.write(reg);
	Wire.write(temp, (uint8_t) 4);
	Wire.endTransmission();

}

/** Send to the encoder 3 byte **/
void i2cEncoderLibV2::writeEncoder24bit(uint8_t reg, uint32_t data) {
	uint8_t temp[3];
	_tem_data.val = data;
	temp[0] = _tem_data.bval[2];
	temp[1] = _tem_data.bval[1];
	temp[2] = _tem_data.bval[0];
	Wire.beginTransmission(_add);
	Wire.write(reg);
	Wire.write(temp, (uint8_t) 3);
	Wire.endTransmission();

}
