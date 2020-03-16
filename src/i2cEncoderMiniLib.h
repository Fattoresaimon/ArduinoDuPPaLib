//
//    FILE: i2cEncoderMiniLib.h
// VERSION: 0.1..
// PURPOSE: Libreary for the i2c encoder board with arduinp
// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
//
// DATASHEET:
//
//     URL:
//
// AUTHOR:
// Simone Caron
//

#ifndef i2cEncoderMiniLib_H
#define i2cEncoderMiniLib_H

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

class i2cEncoderMiniLib {
  public:

    /*Encoder register definition*/
    enum I2C_Register {
      REG_GCONF = 0x00,
      REG_INTCONF = 0x01,
      REG_ESTATUS = 0x02,
      REG_CVALB4 = 0x03,
      REG_CVALB3 = 0x04,
      REG_CVALB2 = 0x05,
      REG_CVALB1 = 0x06,
      REG_CMAXB4 = 0x07,
      REG_CMAXB3 = 0x08,
      REG_CMAXB2 = 0x09,
      REG_CMAXB1 = 0x0A,
      REG_CMINB4 = 0x0B,
      REG_CMINB3 = 0x0C,
      REG_CMINB2 = 0x0D,
      REG_CMINB1 = 0x0E,
      REG_ISTEPB4 = 0x0F,
      REG_ISTEPB3 = 0x10,
      REG_ISTEPB2 = 0x11,
      REG_ISTEPB1 = 0x12,
      REG_DPPERIOD = 0x13,
      REG_ADDRESS = 0x14,
      REG_IDCODE = 0x70,
      REG_VERSION = 0x71,
      REG_I2CADDRESS = 0x72,
      REG_EEPROMS = 0x81,
    } I2C1_REGISTER;


    /* Encoder configuration bit. Use with GCONF */
    enum GCONF_PARAMETER {
      WRAP_ENABLE = 0x01,
      WRAP_DISABLE = 0x00,
      DIRE_LEFT = 0x02,
      DIRE_RIGHT = 0x00,
      IPUP_ENABLE = 0x04,
      IPUP_DISABLE = 0x00,
      RMOD_X4 = 0x10,
      RMOD_X2 = 0x08,
      RMOD_X1 = 0x00,

      RESET = 0x80,
    };

    /* Encoder status bits and setting. Use with: INTCONF for set and with ESTATUS for read the bits  */
    enum Int_Status {
      PUSHR = 0x01,
      PUSHP = 0x02,
      PUSHD = 0x04,
      PUSHL = 0x08,
      RINC = 0x10,
      RDEC = 0x20,
      RMAX = 0x40,
      RMIN = 0x80,
    };

    union Data_v {
      int32_t val;
      uint8_t bval[4];
    };

    uint8_t id = 0x00;
	uint8_t address = 0x00;
    typedef void (*Callback)(i2cEncoderMiniLib*);

    /* Event */
    Callback onButtonRelease = NULL;
    Callback onButtonPush = NULL;
    Callback onButtonDoublePush = NULL;
    Callback onButtonLongPush = NULL;
    Callback onIncrement = NULL;
    Callback onDecrement = NULL;
    Callback onChange = NULL;
    Callback onMax = NULL;
    Callback onMin = NULL;
    Callback onMinMax = NULL;


    /** Configuration methods **/
    i2cEncoderMiniLib(uint8_t add);
    void begin(uint8_t conf);
    void reset(void);
    void autoconfigInterrupt(void);

    /**    Read functions   **/
    uint8_t readInterruptConfig(void);

    /** Status function **/
    bool updateStatus(void);
    bool readStatus(Int_Status s);
    uint8_t readStatus(void);

    /** Encoder functions **/
    int32_t readCounterLong(void);
    int16_t readCounterInt(void);
    int8_t readCounterByte(void);
    int32_t readMax(void);
    int32_t readMin(void);
    int32_t readStep(void);

    /** Timing registers **/
    uint8_t readAntibouncingPeriod(void);
    uint8_t readDoublePushPeriod(void);
    uint8_t readIDCode(void);
    uint8_t readVersion(void);

    /** EEPROM register **/
    uint8_t readEEPROM(uint8_t add);

    /******    Write functions   ********/
    void writeInterruptConfig(uint8_t interrupt);

    /** Encoder functions **/
    void writeCounter(int32_t counter);
    void writeMax(int32_t max);
    void writeMin(int32_t min);
    void writeStep(int32_t step);

    /** Timing registers **/
    void writeAntibouncingPeriod(uint8_t bounc);
    void writeDoublePushPeriod(uint8_t dperiod);
    void ChangeI2CAddress(uint8_t add);
    /** EEPROM register **/
    void writeEEPROM(uint8_t add, uint8_t data);

  private:

    
    uint8_t _stat = 0x00;
    uint8_t _gconf = 0x00;
    union Data_v _tem_data;

    void eventCaller(Callback *event);
    uint8_t readEncoderByte(uint8_t reg);
    int16_t readEncoderInt(uint8_t reg);
    int32_t readEncoderLong(uint8_t reg);
    void writeEncoder(uint8_t reg, uint8_t data);
    void writeEncoder(uint8_t reg, int32_t data);
};

#endif
