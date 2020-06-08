//
//    FILE: LEDRing.c
// VERSION: 1.0..
// PURPOSE: Library for LEDRing from DUPPA
// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
//
// DATASHEET:
//
//     URL:
//
// AUTHOR:
// Simone Caron
//

#include "LEDRing.h"
#include <Wire.h>

LEDRing::LEDRing(uint8_t add) {
  _add = add;
}


void LEDRing::PWM_MODE(void) {
  selectBank(ISSI_PAGE2);
}

void  LEDRing::LEDRing_Reset(void) {
  selectBank(ISSI_PAGE4);
  Wire.beginTransmission(_add);
  Wire.write((byte)RESET_REG);
  Wire.endTransmission();
  Wire.requestFrom(_add, (uint8_t) 1);
  Wire.endTransmission();

}

void LEDRing::Configuration(uint8_t conf) {
  selectBank(ISSI_PAGE4);
  writeRegister8(ISSI_CONFIGURATION, conf);
}

void LEDRing::PULLUP(uint8_t pull) {
  selectBank(ISSI_PAGE4);
  writeRegister8(ISSI_PULLUP, pull);
}

void LEDRing::PULLDOWN(uint8_t pull) {
  selectBank(ISSI_PAGE4);
  writeRegister8(ISSI_PULLDOWN, pull);
}


void LEDRing::GlobalCurrent(uint8_t curr) {
  selectBank(ISSI_PAGE4);
  writeRegister8(ISSI_GLOBALCURRENT, curr);
}


void LEDRing::EnableAllOutput(void) {
  selectBank(ISSI_PAGE1);
  for (uint8_t i = 0; i < 0x24; i++) {
    writeRegister8(i, 0xff);
  }
}

void  LEDRing::LEDRing_Set_RGB(uint8_t led_n, uint32_t color) {

  writeRegister8(issi_led_map[0][led_n], ((color >> 16) & 0xFF));
  writeRegister8(issi_led_map[1][led_n], ((color >> 8) & 0xFF));
  writeRegister8(issi_led_map[2][led_n], (color & 0xFF));

  //  uint8_t temp_add;
  //  if (led_n < 12) {
  //    temp_add = issi_led_map[led_n];
  //    writeRegister8((temp_add | 0xB0), ((color >> 16) & 0xFF));
  //    writeRegister8((temp_add | 0xA0), ((color >> 8) & 0xFF));
  //    writeRegister8((temp_add | 0x90), (color & 0xFF));
  //    return;
  //  }
  //
  //  if (led_n < 24) {
  //    temp_add = issi_led_map[led_n - 12];
  //    writeRegister8((temp_add | 0x80), ((color >> 16) & 0xFF));
  //    writeRegister8((temp_add | 0x70), ((color >> 8) & 0xFF));
  //    writeRegister8((temp_add | 0x60), (color & 0xFF));
  //    return;
  //  }
  //
  //  if (led_n < 36) {
  //    temp_add = issi_led_map[led_n - 24];
  //    writeRegister8((temp_add | 0x50), ((color >> 16) & 0xFF));
  //    writeRegister8((temp_add | 0x40), ((color >> 8) & 0xFF));
  //    writeRegister8((temp_add | 0x30), (color & 0xFF));
  //    return;
  //  }
  //  if (led_n < 48) {
  //    temp_add = issi_led_map[led_n - 36];
  //    writeRegister8((temp_add | 0x20), ((color >> 16) & 0xFF));
  //    writeRegister8((temp_add | 0x10), ((color >> 8) & 0xFF));
  //    writeRegister8(temp_add, (color & 0xFF));
  //    return;
  //  }
  //
}


void  LEDRing::LEDRing_Set_RED(uint8_t led_n, uint8_t color) {

  writeRegister8(issi_led_map[0][led_n], color);

  //  uint8_t temp_add;
  //  if (led_n < 12) {
  //    temp_add = issi_led_map[led_n];
  //    writeRegister8((temp_add | 0xB0), color);
  //    return;
  //  }
  //
  //  if (led_n < 24) {
  //    temp_add = issi_led_map[led_n - 12];
  //    writeRegister8((temp_add | 0x80), color);
  //    return;
  //  }
  //
  //  if (led_n < 36) {
  //    temp_add = issi_led_map[led_n - 24];
  //    writeRegister8((temp_add | 0x50), color);
  //    return;
  //  }
  //  if (led_n < 48) {
  //    temp_add = issi_led_map[led_n - 36];
  //    writeRegister8((temp_add | 0x20), color);
  //    return;
  //  }
}

void  LEDRing::LEDRing_Set_GREEN(uint8_t led_n, uint8_t color) {

  writeRegister8(issi_led_map[1][led_n], color);

  //  uint8_t temp_add;
  //  if (led_n < 12) {
  //    temp_add = issi_led_map[led_n];
  //    writeRegister8((temp_add | 0xA0), color);
  //    return;
  //  }
  //
  //  if (led_n < 24) {
  //    temp_add = issi_led_map[led_n - 12];
  //    writeRegister8((temp_add | 0x70), color);
  //    return;
  //  }
  //
  //  if (led_n < 36) {
  //    temp_add = issi_led_map[led_n - 24];
  //    writeRegister8((temp_add | 0x40), color);
  //    return;
  //  }
  //  if (led_n < 48) {
  //    temp_add = issi_led_map[led_n - 36];
  //    writeRegister8((temp_add | 0x10), color);
  //    return;
  //  }
}

void  LEDRing::LEDRing_Set_BLUE(uint8_t led_n, uint8_t color) {

  writeRegister8(issi_led_map[2][led_n], color);

  //  uint8_t temp_add;
  //  if (led_n < 12) {
  //    temp_add = issi_led_map[led_n];
  //    writeRegister8((temp_add | 0x90), color);
  //    return;
  //  }
  //
  //  if (led_n < 24) {
  //    temp_add = issi_led_map[led_n - 12];
  //    writeRegister8((temp_add | 0x60), color);
  //    return;
  //  }
  //
  //  if (led_n < 36) {
  //    temp_add = issi_led_map[led_n - 24];
  //    writeRegister8((temp_add | 0x30), color);
  //    return;
  //  }
  //  if (led_n < 48) {
  //    temp_add = issi_led_map[led_n - 36];
  //    writeRegister8(temp_add, color);
  //    return;
  //  }
}


void  LEDRing::ClearAll(void) {
  uint8_t   buff[6] = {0};
  uint8_t i = 0x00;
  PWM_MODE();
  while (i <= 0xB0) {
    writeBuff(i, buff, 6);
    writeBuff((i + 0x08), buff, 6);
    i = i + 0x10;
  }
}

void  LEDRing::selectBank(uint8_t b) {
  writeRegister8(ISSI_COMMANDREGISTER_LOCK, ISSI_ULOCK_CODE);
  writeRegister8(ISSI_COMMANDREGISTER, b);
}

void LEDRing::writeRegister8(uint8_t reg, uint8_t data) {
  Wire.beginTransmission(_add);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}

void LEDRing::writeBuff(uint8_t reg, uint8_t *data, uint8_t dim) {
  Wire.beginTransmission(_add);
  Wire.write(reg);
  Wire.write(data, dim);
  Wire.endTransmission();

}
