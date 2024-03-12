//
//    FILE: LEDRingSmall.c
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



#include "LEDRingSmall.h"
#include <Wire.h>

LEDRingSmall::LEDRingSmall(uint8_t add) {
  _add = add;
}

void LEDRingSmall::LEDRingSmall_PWM_MODE(void) {
  selectBank(ISSI3746_PAGE0);
}

void LEDRingSmall::LEDRingSmall_Configuration(uint8_t conf) {
  selectBank(ISSI3746_PAGE1);
  writeRegister8(ISSI3746_CONFIGURATION, conf);
}

void LEDRingSmall::LEDRingSmall_SetScaling(uint8_t led_n, uint8_t scal) {
  selectBank(ISSI3746_PAGE1);
  writeRegister8(led_n, scal);
}

void LEDRingSmall::LEDRingSmall_SetScaling(uint8_t scal) {
  selectBank(ISSI3746_PAGE1);

  for (uint8_t i = 1; i < 73; i++) {
    writeRegister8(i, scal);
  }
}

void LEDRingSmall::LEDRingSmall_GlobalCurrent(uint8_t curr) {
  selectBank(ISSI3746_PAGE1);
  writeRegister8(ISSI3746_GLOBALCURRENT, curr);
}

void LEDRingSmall::LEDRingSmall_PULLUP_DOWN(uint8_t pull) {
  selectBank(ISSI3746_PAGE1);
  writeRegister8(ISSI3746_PULLUPDOWM, pull);
}


uint8_t LEDRingSmall::LEDRingSmall_Temperature(void) {
  selectBank(ISSI3746_PAGE1);
  return(readRegister8(ISSI3746_TEMPERATURE));
}

void LEDRingSmall::LEDRingSmall_SpreadSpectrum(uint8_t spread) {
  selectBank(ISSI3746_PAGE1);
  writeRegister8(ISSI3746_SPREADSPECTRUM, spread);
}

void LEDRingSmall::LEDRingSmall_Reset(void) {
  selectBank(ISSI3746_PAGE1);
  writeRegister8(ISSI3746_RESET_REG, 0xAE);
}

void LEDRingSmall::LEDRingSmall_PWMFrequencyEnable(uint8_t PWMenable) {
  selectBank(ISSI3746_PAGE1);
  writeRegister8(ISSI3746_PWM_FREQUENCY_ENABLE, PWMenable);
}

void LEDRingSmall::LEDRingSmall_PWMFrequencySetting(uint8_t pwmfreq) {
  selectBank(ISSI3746_PAGE1);
  writeRegister8(ISSI3746_PWM_FREQUENCY_SET, pwmfreq);
}


void  LEDRingSmall::LEDRingSmall_Set_RGB(uint8_t led_n, uint32_t color) {

  writeRegister8(issi_led_map[0][led_n], ((color >> 16) & 0xFF));
  writeRegister8(issi_led_map[1][led_n], ((color >> 8) & 0xFF));
  writeRegister8(issi_led_map[2][led_n], (color & 0xFF));
}

void  LEDRingSmall::LEDRingSmall_Set_RED(uint8_t led_n, uint8_t color) {

  writeRegister8(issi_led_map[0][led_n], color);
}

void  LEDRingSmall::LEDRingSmall_Set_GREEN(uint8_t led_n, uint8_t color) {

  writeRegister8(issi_led_map[1][led_n], color);

}

void  LEDRingSmall::LEDRingSmall_Set_BLUE(uint8_t led_n, uint8_t color) {

  writeRegister8(issi_led_map[2][led_n], color);
}

void  LEDRingSmall::LEDRingSmall_ClearAll(void) {
  uint8_t i;
  LEDRingSmall_PWM_MODE();
  
  for (i = 1; i < 73; i++) {
   	  writeRegister8(i,0);
  }

}
  

void  LEDRingSmall::selectBank(uint8_t b) {
  writeRegister8(ISSI3746_COMMANDREGISTER_LOCK, ISSI3746_ULOCK_CODE);
  writeRegister8(ISSI3746_COMMANDREGISTER, b);
}

void LEDRingSmall::writeRegister8(uint8_t reg, uint8_t data) {
  Wire.beginTransmission(_add);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}

void LEDRingSmall::writeBuff(uint8_t reg, uint8_t *data, uint8_t dim) {
  Wire.beginTransmission(_add);
  Wire.write(reg);
  Wire.write(data, dim);
  Wire.endTransmission();
}

uint8_t LEDRingSmall::readRegister8(uint8_t reg) {
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
