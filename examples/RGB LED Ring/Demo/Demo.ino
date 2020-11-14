#include <Wire.h>
#include <LEDRing.h>


/*
  Connections with Arduino UNO:
  VCC -> 5V
  GND -> GND
  VIO -> 5V
  SDA -> A4
  SCL -> A5
  INT -> //Not used now
*/

const  uint32_t fade1_table[48] = {0x8011EE, 0x900AE5, 0xA004DA, 0xB001CD, 0xBF00BF, 0xCD01B0, 0xDA04A0, 0xE50A90, 0xEE1180, 0xF51A6F, 0xFB255F, 0xFE324F, 0xFF4040, 0xFE4F32, 0xFB5F25, 0xF56F1A, 0xEE8011, 0xE5900A, 0xDAA004, 0xCDB001, 0xBFBF00, 0xB0CD01, 0xA0DA04, 0x90E50A, 0x80EE11, 0x6FF51A, 0x5FFB25, 0x4FFE32, 0x40FF40, 0x32FE4F, 0x25FB5F, 0x1AF56F, 0x11EE80, 0x0AE590, 0x04DAA0, 0x01CDB0, 0x00BFBF, 0x01B0CD, 0x04A0DA, 0x0A90E5, 0x1180EE, 0x1A6FF5, 0x255FFB, 0x324FFE, 0x4040FF, 0x4F32FE, 0x5F25FB, 0x6F1AF5, };
const  uint32_t fade2_table[48] = {0xFF0000, 0xFF0001, 0xFF0004, 0xFF000E, 0xFF0020, 0xFF003E, 0xFF006B, 0xFF00AB, 0xFF00FF, 0xAB00FF, 0x6B00FF, 0x3E00FF, 0x2000FF, 0x0E00FF, 0x0400FF, 0x0100FF, 0x0000FF, 0x0001FF, 0x0004FF, 0x000EFF, 0x0020FF, 0x003EFF, 0x006BFF, 0x00ABFF, 0x00FFFF, 0x00FFAB, 0x00FF6B, 0x00FF3E, 0x00FF20, 0x00FF0E, 0x00FF04, 0x00FF01, 0x00FF00, 0x01FF00, 0x04FF00, 0x0EFF00, 0x20FF00, 0x3EFF00, 0x6BFF00, 0xABFF00, 0xFFFF00, 0xFFAB00, 0xFF6B00, 0xFF3E00, 0xFF2000, 0xFF0E00, 0xFF0400, 0xFF0100, };
uint8_t j = 0;
uint8_t i = 0;
uint16_t tim = 0;
uint8_t r = 5, g = 8, b = 20, rg = 40, br = 35, bg = 17;


LEDRing LEDRing(0x5A); // change the adress according to your board.


void setup(void) {

  Wire.begin();

  LEDRing.LEDRing_Reset();
  delay(20);

  LEDRing.LEDRing_EnableAllOutput();
  LEDRing.LEDRing_Configuration(0x01); //Normal operation
  LEDRing.LEDRing_GlobalCurrent(0xff); // maximum current output
  LEDRing.LEDRing_PULLUP(0b111); // 32k pull-up resistor for avoid ghost effect
  LEDRing.LEDRing_PULLDOWN(0b111); // 32k pull-down resistor for avoid ghost effect
  LEDRing.LEDRing_PWM_MODE(); //Exit to the configuration mode and enter in PWM mode

}

void loop() {

  for (i = 0; i < 48; i++) {
    LEDRing.LEDRing_Set_RED(i, 0xff);
    delay(15);
  }

  for (i = 0; i < 48; i++) {
    LEDRing.LEDRing_Set_RED(i, 0);
    LEDRing.LEDRing_Set_GREEN(i, 0xff);
    delay(15);
  }

  for (i = 0; i < 48; i++) {
    LEDRing.LEDRing_Set_GREEN(i, 0);
    LEDRing.LEDRing_Set_BLUE(i, 0xff);
    delay(15);
  }

  for (i = 0; i < 48; i++) {

    LEDRing.LEDRing_Set_RGB(i, 0x6600CC);
    delay(15);
  }

  for (i = 0; i < 48; i++) {

    LEDRing.LEDRing_Set_RGB(i, 0xFF9933);
    delay(15);
  }
  LEDRing.LEDRing_ClearAll();


  for (tim = 0; tim < 2000; tim++) {



    if ((tim % 5) == 0) {
      LEDRing.LEDRing_Set_RED(r, 0);
      r = r + 1;
      if (r >= 48)
        r = 0;
      LEDRing.LEDRing_Set_RED(r, 0xFF);
    }

    if ((tim % 6) == 0) {
      LEDRing.LEDRing_Set_GREEN(g, 0);
      g = g + 1;
      if (g >= 48)
        g = 0;
      LEDRing.LEDRing_Set_GREEN(g, 0xFF);
    }

    if ((tim % 8) == 0) {
      LEDRing.LEDRing_Set_BLUE(b, 0);
      b = b + 1;
      if (b >= 48)
        b = 0;
      LEDRing.LEDRing_Set_BLUE(b, 0xFF);
    }


    if ((tim % 7) == 0) {
      LEDRing.LEDRing_Set_BLUE(br, 0);
      LEDRing.LEDRing_Set_RED(br, 0);
      br = br + 1;
      if (br >= 48)
        br = 0;
      LEDRing.LEDRing_Set_BLUE(br, 0xFF);
      LEDRing.LEDRing_Set_RED(br, 0xFF);
    }


    if ((tim % 10) == 0) {
      LEDRing.LEDRing_Set_BLUE(bg, 0);
      LEDRing.LEDRing_Set_GREEN(bg, 0);
      bg = bg + 1;
      if (bg >= 48)
        bg = 0;
      LEDRing.LEDRing_Set_BLUE(bg, 0xFF);
      LEDRing.LEDRing_Set_GREEN(bg, 0xFF);
    }

    if ((tim % 11) == 0) {
      LEDRing.LEDRing_Set_RED(rg, 0);
      LEDRing.LEDRing_Set_GREEN(rg, 0);
      rg = rg + 1;
      if (rg >= 48)
        rg = 0;
      LEDRing.LEDRing_Set_RED(rg, 0xFF);
      LEDRing.LEDRing_Set_GREEN(rg, 0xFF);
    }

    delay(5);
  }


  for (tim = 0; tim < 100; tim++) {
    j = tim % 48;

    for (i = 0; i < 48; i++) {
      LEDRing.LEDRing_Set_RGB(i, fade1_table[j]);
      j++;
      if (j >= 48)
        j = 0;
    }
    delay(20);
  }

  for (tim = 0; tim < 100; tim++) {
    j = tim % 48;
    for (i = 0; i < 48; i++) {
      LEDRing.LEDRing_Set_RGB(i, fade2_table[j]);
      j++;
      if (j >= 48)
        j = 0;
    }
    delay(20);
  }

  for (i = 0xff; i > 0; i--) {

    LEDRing.LEDRing_GlobalCurrent(i);
    delay(10);
  }
  LEDRing.LEDRing_ClearAll();
  LEDRing.LEDRing_GlobalCurrent(0xff);
  delay(1000);
  LEDRing.LEDRing_PWM_MODE(); //Exit to the configuration mode and enter in PWM mode

}
