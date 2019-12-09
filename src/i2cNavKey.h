//
//    FILE: i2cNavKey.h
// VERSION: 0.1..
// PURPOSE: Libreary for the i2c NavKey board with arduinp
// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
//
// DATASHEET:
//
//     URL:
//
// AUTHOR:
// Simone Caron
//

#ifndef i2cNavKey_H
#define i2cNavKey_H

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

class i2cNavKey {
public:
	/*NavKey register definition*/
	enum I2C_Register {
		REG_GCONF = 0x00,
		REG_GP1CONF = 0x01,
		REG_GP2CONF = 0x02,
		REG_GP3CONF = 0x03,
		REG_INTCONFB2 = 0x04,
		REG_INTCONFB1 = 0x05,
		REG_STATUSB2 = 0x06,
		REG_STATUSB1 = 0x07,
		REG_SSTATUS = 0x08,
		REG_FSTATUS = 0x09,
		REG_CVALB4 = 0x0A,
		REG_CVALB3 = 0x0B,
		REG_CVALB2 = 0x0C,
		REG_CVALB1 = 0x0D,
		REG_CMAXB4 = 0x0E,
		REG_CMAXB3 = 0x0F,
		REG_CMAXB2 = 0x10,
		REG_CMAXB1 = 0x11,
		REG_CMINB4 = 0x12,
		REG_CMINB3 = 0x13,
		REG_CMINB2 = 0x14,
		REG_CMINB1 = 0x15,
		REG_ISTEPB4 = 0x16,
		REG_ISTEPB3 = 0x17,
		REG_ISTEPB2 = 0x18,
		REG_ISTEPB1 = 0x19,
		REG_GP1REG = 0x1A,
		REG_GP2REG = 0x1B,
		REG_GP3REG = 0x1C,
		REG_DPPERIOD = 0x1D,
		REG_FADEGP = 0x1E,
		REG_GAMMAGP1 = 0x1F,
		REG_GAMMAGP2 = 0x20,
		REG_GAMMAGP3 = 0x21,
		REG_IDCODE = 0x70,
		REG_VERSION = 0x71,
		REG_EEPROMS = 0x80,
	};

	/* NavKey configuration bit. Use with GCONF */
	enum GCONF_PARAMETER {
		FLOAT_DATA = 0x01,
		INT_DATA = 0x00,
		WRAP_ENABLE = 0x02,
		WRAP_DISABLE = 0x00,
		DIRE_LEFT = 0x04,
		DIRE_RIGHT = 0x00,
		IPUP_DISABLE = 0x08,
		IPUP_ENABLE = 0x00,
		CLK_STRECH_ENABLE = 0x10,
		CLK_STRECH_DISABLE = 0x00,
		EEPROM_BANK1 = 0x20,
		EEPROM_BANK2 = 0x00,
		RESET = 0x80,
	};

	/* NavKey status bits and setting. Use with: INTCONF for set and with ESTATUS for read the bits  */
	enum Int_Status {
		UPR = 0x0001,
		UPP = 0x0002,
		DNR = 0x0004,
		DNP = 0x0008,
		RTR = 0x0010,
		RTP = 0x0020,
		LTR = 0x0040,
		LTP = 0x0080,
		CTRR = 0x0100,
		CTRP = 0x0200,
		CTRDP = 0x0400,
		RINC = 0x0800,
		RDEC = 0x1000,
		RMAX = 0x2000,
		RMIN = 0x4000,
		INT_2 = 0x8000,
	};

	/* NavKey Int2 bits. Use to read the bits of I2STATUS  */
	enum Int2_Status {
		GP1_POS = 0x01,
		GP1_NEG = 0x02,
		GP2_POS = 0x04,
		GP2_NEG = 0x08,
		GP3_POS = 0x10,
		GP3_NEG = 0x20,
		FADE_INT = 0x40,
	};

	/* NavKey Fade status bits. Use to read the bits of FSTATUS  */
	enum Fade_Status {
		FADES_GP1 = 0x01, FADES_GP2 = 0x02, FADES_GP3 = 0x04,
	};

	/* GPIO Configuration. USe with GP1CONF,GP2CONF,GP3CONF */
	enum GP_PARAMETER {
		GP_PWM = 0x00,
		GP_OUT = 0x01,
		GP_AN = 0x02,
		GP_IN = 0x03,
		GP_PULL_EN = 0x04,
		GP_PULL_DI = 0x00,
		GP_INT_DI = 0x00,
		GP_INT_PE = 0x08,
		GP_INT_NE = 0x10,
		GP_INT_BE = 0x18,
	};

	typedef enum {
		GAMMA_1 = 0,
		GAMMA_1_8 = 1,
		GAMMA_2 = 2,
		GAMMA_2_2 = 3,
		GAMMA_2_4 = 4,
		GAMMA_2_6 = 5,
		GAMMA_2_8 = 6,

	} GAMMA_PARAMETER;

	union Data_v {
		float fval;
		uint16_t wval;
		int32_t val;
		uint8_t bval[4];
	};

	typedef void (*Callback)(i2cNavKey*);

	/* Event */
	Callback onArrowsPush = NULL;
	Callback onArrowsRelese = NULL;
	Callback onUpPush = NULL;
	Callback onUpRelease = NULL;
	Callback onDownPush = NULL;
	Callback onDownRelease = NULL;
	Callback onRightPush = NULL;
	Callback onRightRelease = NULL;
	Callback onLeftPush = NULL;
	Callback onLeftRelease = NULL;
	Callback onCentralPush = NULL;
	Callback onCentralRelease = NULL;
	Callback onCentralDoublePush = NULL;
	Callback onIncrement = NULL;
	Callback onDecrement = NULL;
	Callback onChange = NULL;
	Callback onMax = NULL;
	Callback onMin = NULL;
	Callback onMinMax = NULL;
	Callback onGP1Rise = NULL;
	Callback onGP1Fall = NULL;
	Callback onGP2Rise = NULL;
	Callback onGP2Fall = NULL;
	Callback onGP3Rise = NULL;
	Callback onGP3Fall = NULL;
	Callback onFadeProcess = NULL;

	i2cNavKey(uint8_t add);
	void begin(uint8_t conf);
	void reset(void);
	void autoconfigInterrupt(void);

	/**    Read functions   **/
	/** Configuration function **/
	uint8_t readGP1conf(void);
	uint8_t readGP2conf(void);
	uint8_t readGP3conf(void);
	uint16_t readInterruptConfig(void);

	/** Status function **/
	bool updateStatus(void);
	bool readStatus(Int_Status s);
	uint16_t readStatus(void);

	bool readInt2(Int2_Status s);
	uint8_t readInt2(void);

	bool readFadeStatus(Fade_Status s);
	uint8_t readFadeStatus(void);

	/** NavKey functions **/
	float readCounterFloat(void);
	int32_t readCounterLong(void);
	int16_t readCounterInt(void);
	int8_t readCounterByte(void);

	int32_t readMax(void);
	float readMaxFloat(void);

	int32_t readMin(void);
	float readMinFloat(void);

	int32_t readStep(void);
	float readStepFloat(void);

	/** GP LED Functions **/
	uint8_t readGP1(void);
	uint8_t readGP2(void);
	uint8_t readGP3(void);

	/** Timing registers **/
	uint8_t readDoublePushPeriod(void);
	uint8_t readFadeGP(void);

	/** EEPROM register **/
	uint8_t readEEPROM(uint8_t add);

	/** **/
	uint8_t readIDCode(void);
	uint8_t readVersion(void);

	uint8_t readNavKeyByte(uint8_t reg);
	int16_t readNavKeyInt(uint8_t reg);
	int32_t readNavKeyLong(uint8_t reg);

	/******    Write functions   ********/
	void writeGP1conf(uint8_t gp1);
	void writeGP2conf(uint8_t gp2);
	void writeGP3conf(uint8_t gp3);
	void writeInterruptConfig(uint16_t interrupt);

	/** NavKey functions **/
	void writeCounter(int32_t counter);
	void writeCounter(float counter);

	void writeMax(int32_t max);
	void writeMax(float max);

	void writeMin(int32_t min);
	void writeMin(float min);

	void writeStep(int32_t step);
	void writeStep(float step);

	/** GP LED Functions **/
	void writeGP1(uint8_t gp1);
	void writeGP2(uint8_t gp2);
	void writeGP3(uint8_t gp3);

	/** Timing registers **/
	void writeDoublePushPeriod(uint8_t dperiod);
	void writeFadeGP(uint8_t fade);

	void writeGammaGP1(GAMMA_PARAMETER Gamma);
	void writeGammaGP2(GAMMA_PARAMETER Gamma);
	void writeGammaGP3(GAMMA_PARAMETER Gamma);

	/** EEPROM register **/
	void writeEEPROM(uint8_t add, uint8_t data);

private:

	uint8_t _add = 0x00;
	uint16_t _stat = 0x00;
	uint8_t _stat2 = 0x00;
	uint8_t _statfade = 0x00;
	uint8_t _gconf = 0x00;
	union Data_v _tem_data;

	void eventCaller(Callback *event);
	float readNavKeyFloat(uint8_t reg);
	void writeNavKey(uint8_t reg, uint8_t data);
	void writeNavKey(uint8_t reg, uint16_t data);
	void writeNavKey(uint8_t reg, int32_t data);
	void writeNavKey(uint8_t reg, float data);
	void writeNavKey24bit(uint8_t reg, uint32_t data);

};

#endif
