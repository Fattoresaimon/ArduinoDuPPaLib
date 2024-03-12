//
//    FILE: LEDRingSmall.h
// VERSION: 1.2..
// PURPOSE: Library for LEDRingSmall from DUPPA. chip used  IS31FL3746A
// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
//
// DATASHEET: https://www.lumissil.com/assets/pdf/core/IS31FL3746A_DS.pdf
//
//     URL:
//
// AUTHOR:
// Simone Caron
//


#ifndef LEDRingSmall_H
#define LEDRingSmall_H

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#define ISSI3746_PAGE0 0x00
#define ISSI3746_PAGE1 0x01


#define ISSI3746_COMMANDREGISTER 0xFD
#define ISSI3746_COMMANDREGISTER_LOCK 0xFE
#define ISSI3746_ID_REGISTER 0xFC
#define ISSI3746_ULOCK_CODE 0xC5

#define ISSI3746_CONFIGURATION 0x50
#define ISSI3746_GLOBALCURRENT 0x51
#define ISSI3746_PULLUPDOWM 0x52
#define ISSI3746_OPENSHORT 0x53
#define ISSI3746_TEMPERATURE 0x5F
#define ISSI3746_SPREADSPECTRUM 0x60
#define ISSI3746_RESET_REG 0x8F
#define ISSI3746_PWM_FREQUENCY_ENABLE 0xE0
#define ISSI3746_PWM_FREQUENCY_SET 0xE2


#define ISSI3746_SJ1 0b1100000
#define ISSI3746_SJ2 0b1100001
#define ISSI3746_SJ3 0b1100010
#define ISSI3746_SJ4 0b1100011
#define ISSI3746_SJ5 0b1100000
#define ISSI3746_SJ6 0b1100100
#define ISSI3746_SJ7 0b1101000
#define ISSI3746_SJ8 0b1101100


class LEDRingSmall {
  public:

    LEDRingSmall(uint8_t add);
    void LEDRingSmall_PWM_MODE(void);
    void LEDRingSmall_Configuration(uint8_t conf);
    void LEDRingSmall_SetScaling(uint8_t led_n, uint8_t scal);
    void LEDRingSmall_SetScaling(uint8_t scal);
    void LEDRingSmall_GlobalCurrent(uint8_t curr);
    void LEDRingSmall_PULLUP_DOWN(uint8_t pull);
    uint8_t LEDRingSmall_Temperature(void);
    void LEDRingSmall_SpreadSpectrum(uint8_t spread);
    void LEDRingSmall_Reset(void);
    void LEDRingSmall_PWMFrequencyEnable(uint8_t PWMenable);
    void LEDRingSmall_PWMFrequencySetting(uint8_t pwmfreq);

    void LEDRingSmall_Set_RGB(uint8_t led_n, uint32_t color);
    void LEDRingSmall_Set_RED(uint8_t led_n, uint8_t color);
    void LEDRingSmall_Set_GREEN(uint8_t led_n, uint8_t color);
    void LEDRingSmall_Set_BLUE(uint8_t led_n, uint8_t color);
    void LEDRingSmall_ClearAll(void);

  private:
    const uint8_t issi_led_map[3][24] = {
      {0x48, 0x36, 0x24, 0x12, 0x45, 0x33, 0x21, 0x0F, 0x42, 0x30, 0x1E, 0x0C, 0x3F, 0x2D, 0x1B, 0x09, 0x3C, 0x2A, 0x18, 0x06, 0x39, 0x27, 0x15, 0x03}, // Red
      {0x47, 0x35, 0x23, 0x11, 0x44, 0x32, 0x20, 0x0E, 0x41, 0x2F, 0x1D, 0x0B, 0x3E, 0x2C, 0x1A, 0x08, 0x3B, 0x29, 0x17, 0x05, 0x38, 0x26, 0x14, 0x02}, //Green
      {0x46, 0x34, 0x22, 0x10, 0x43, 0x31, 0x1F, 0x0D, 0x40, 0x2E, 0x1C, 0x0A, 0x3D, 0x2B, 0x19, 0x07, 0x3A, 0x28, 0x16, 0x04, 0x37, 0x25, 0x13, 0x01}, //Blue
    };

    uint8_t _add;
    void  selectBank(uint8_t b);
    void  writeRegister8(uint8_t reg, uint8_t data);
    void  writeBuff(uint8_t reg, uint8_t *data, uint8_t dim);
    uint8_t readRegister8(uint8_t reg);
};
#endif
