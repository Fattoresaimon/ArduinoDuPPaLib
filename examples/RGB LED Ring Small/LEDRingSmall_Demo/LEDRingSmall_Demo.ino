#include <Wire.h>
#include "LEDRingSmall.h"

/*
Board Pinout
+-----+--------+----------+-------------+
| Pin | Color  | Function | Arduino pin |
+-----+--------+----------+-------------+
|   1 | Red    | VCC      | +5V         |
|   2 | Black  | GND      | GND         |
|   3 | Yellow | VIO      | +5V         |
|   4 | Green  | SDA      | A4          |
|   5 | Blue   | SCL      | A5          |
+-----+--------+----------+-------------+
*/

const  uint32_t fade1_table[24] = {0x8011EE, 0xA004DA,  0xBF00BF, 0xDA04A0, 0xEE1180, 0xFB255F, 0xFF4040, 0xFB5F25, 0xEE8011, 0xDAA004, 0xBFBF00, 0xA0DA04, 0x80EE11, 0x5FFB25, 0x40FF40, 0x25FB5F, 0x11EE80, 0x04DAA0, 0x00BFBF, 0x04A0DA, 0x1180EE, 0x255FFB, 0x4040FF, 0x5F25FB};
const  uint32_t fade2_table[24] = {0xFF0000, 0xFF0004,  0xFF0020, 0xFF006B, 0xFF00FF, 0x6B00FF, 0x2000FF, 0x0400FF, 0x0000FF, 0x0004FF, 0x0020FF, 0x006BFF, 0x00FFFF, 0x00FF6B, 0x00FF20, 0x00FF04, 0x00FF00, 0x04FF00, 0x20FF00, 0x6BFF00, 0xFFFF00, 0xFF6B00, 0xFF2000, 0xFF0400};
static uint8_t j = 0;
static uint8_t i = 0;
static uint16_t tim = 0;
static uint8_t r = 5, g = 8, b = 20, rg = 40, br = 35, bg = 17;

LEDRingSmall LEDRingSmall(ISSI3746_SJ2 | ISSI3746_SJ7);//Only SJ2 and SJ7 are soldered in this example
// Also check if there are the pull-UP resistors on the I2C bus. If no or the board doesn't want to work please also solder the jumper SJ9

void setup(void) {

  Wire.begin();
  Wire.setClock(400000);

  LEDRingSmall.LEDRingSmall_Reset();
  delay(20);

  LEDRingSmall.LEDRingSmall_Configuration(0x01); //Normal operation
  LEDRingSmall.LEDRingSmall_PWMFrequencyEnable(1);
  LEDRingSmall.LEDRingSmall_SpreadSpectrum(0b0010110);
  LEDRingSmall.LEDRingSmall_GlobalCurrent(0x10);
  LEDRingSmall.LEDRingSmall_SetScaling(0xFF);
  LEDRingSmall.LEDRingSmall_PWM_MODE();



}

void loop() {
  LEDRingSmall.LEDRingSmall_GlobalCurrent(0x10);
  LEDRingSmall.LEDRingSmall_PWM_MODE();
    
  for (i = 0; i < 24; i++) {
    LEDRingSmall.LEDRingSmall_Set_RED(i, 0xff);
    delay(30);
  }

  for (i = 0; i < 24; i++) {
    LEDRingSmall.LEDRingSmall_Set_RED(i, 0);
    LEDRingSmall.LEDRingSmall_Set_GREEN(i, 0xff);
    delay(30);
  }

  for (i = 0; i < 24; i++) {
    LEDRingSmall.LEDRingSmall_Set_GREEN(i, 0);
    LEDRingSmall.LEDRingSmall_Set_BLUE(i, 0xff);
    delay(30);
  }

  for (i = 0; i < 24; i++) {

    LEDRingSmall.LEDRingSmall_Set_RGB(i, 0xfc6b03);
    delay(30);
  }

  for (i = 0; i < 24; i++) {

    LEDRingSmall.LEDRingSmall_Set_RGB(i, 0xfc03c6);
    delay(30);
  }
//  delay(2000);
  //LEDRingSmall.LEDRingSmall_ClearAll();




  for (tim = 0; tim < 2000; tim++) {



    if ((tim % 5) == 0) {
      LEDRingSmall.LEDRingSmall_Set_RED(r, 0);
      r = r + 1;
      if (r >= 24)
        r = 0;
      LEDRingSmall.LEDRingSmall_Set_RED(r, 0xFF);
    }

    if ((tim % 6) == 0) {
      LEDRingSmall.LEDRingSmall_Set_GREEN(g, 0);
      g = g + 1;
      if (g >= 24)
        g = 0;
      LEDRingSmall.LEDRingSmall_Set_GREEN(g, 0xFF);
    }

    if ((tim % 8) == 0) {
      LEDRingSmall.LEDRingSmall_Set_BLUE(b, 0);
      b = b + 1;
      if (b >= 24)
        b = 0;
      LEDRingSmall.LEDRingSmall_Set_BLUE(b, 0xFF);
    }


    if ((tim % 7) == 0) {
      LEDRingSmall.LEDRingSmall_Set_BLUE(br, 0);
      LEDRingSmall.LEDRingSmall_Set_RED(br, 0);
      br = br + 1;
      if (br >= 24)
        br = 0;
      LEDRingSmall.LEDRingSmall_Set_BLUE(br, 0xFF);
      LEDRingSmall.LEDRingSmall_Set_RED(br, 0xFF);
    }


    if ((tim % 10) == 0) {
      LEDRingSmall.LEDRingSmall_Set_BLUE(bg, 0);
      LEDRingSmall.LEDRingSmall_Set_GREEN(bg, 0);
      bg = bg + 1;
      if (bg >= 24)
        bg = 0;
      LEDRingSmall.LEDRingSmall_Set_BLUE(bg, 0xFF);
      LEDRingSmall.LEDRingSmall_Set_GREEN(bg, 0xFF);
    }

    if ((tim % 11) == 0) {
      LEDRingSmall.LEDRingSmall_Set_RED(rg, 0);
      LEDRingSmall.LEDRingSmall_Set_GREEN(rg, 0);
      rg = rg + 1;
      if (rg >= 24)
        rg = 0;
      LEDRingSmall.LEDRingSmall_Set_RED(rg, 0xFF);
      LEDRingSmall.LEDRingSmall_Set_GREEN(rg, 0xFF);
    }

    delay(10);
  }


  for (tim = 0; tim < 100; tim++) {
    j = tim % 24;

    for (i = 0; i < 24; i++) {
      LEDRingSmall.LEDRingSmall_Set_RGB(i, fade1_table[j]);
      j++;
      if (j >= 24)
        j = 0;
    }
    delay(40);
  }

  for (tim = 0; tim < 100; tim++) {
    j = tim % 24;
    for (i = 0; i < 24; i++) {
      LEDRingSmall.LEDRingSmall_Set_RGB(i, fade2_table[j]);
      j++;
      if (j >= 24)
        j = 0;
    }
    delay(40);
  }

  for (i = 0xff; i > 0; i--) {

    LEDRingSmall.LEDRingSmall_GlobalCurrent(i);
    delay(20);
  }
  LEDRingSmall.LEDRingSmall_ClearAll();
  LEDRingSmall.LEDRingSmall_GlobalCurrent(0xff);


}
