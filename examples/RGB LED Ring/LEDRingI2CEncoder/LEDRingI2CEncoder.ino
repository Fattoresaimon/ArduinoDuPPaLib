#include <Wire.h>
#include <TimerOne.h>
#include <i2cEncoderLibV2.h>
#include "LEDRing.h"


/*This is a example with the RGB LED Ring plus the I2C Encoder V2.1
   There is a video fo this example: https://youtu.be/wR23IkYd3EY
   For this example you need the library TimerOne, You can do by going Sketch -> IncludeLibrary -> manage librarys and search Timer One
   The timer is used only for some effects... It's not mandatory


  Connections with Arduino UNO:
  - -> GND
  + -> 5V
  SDA -> A4
  SCL -> A5
  INT -> A3
*/


const  uint32_t fade1_table[48] = {0x8011EE, 0x900AE5, 0xA004DA, 0xB001CD, 0xBF00BF, 0xCD01B0, 0xDA04A0, 0xE50A90, 0xEE1180, 0xF51A6F, 0xFB255F, 0xFE324F, 0xFF4040, 0xFE4F32, 0xFB5F25, 0xF56F1A, 0xEE8011, 0xE5900A, 0xDAA004, 0xCDB001, 0xBFBF00, 0xB0CD01, 0xA0DA04, 0x90E50A, 0x80EE11, 0x6FF51A, 0x5FFB25, 0x4FFE32, 0x40FF40, 0x32FE4F, 0x25FB5F, 0x1AF56F, 0x11EE80, 0x0AE590, 0x04DAA0, 0x01CDB0, 0x00BFBF, 0x01B0CD, 0x04A0DA, 0x0A90E5, 0x1180EE, 0x1A6FF5, 0x255FFB, 0x324FFE, 0x4040FF, 0x4F32FE, 0x5F25FB, 0x6F1AF5, };
const  uint32_t fade2_table[48] = {0xFF0000, 0xFF0001, 0xFF0004, 0xFF000E, 0xFF0020, 0xFF003E, 0xFF006B, 0xFF00AB, 0xFF00FF, 0xAB00FF, 0x6B00FF, 0x3E00FF, 0x2000FF, 0x0E00FF, 0x0400FF, 0x0100FF, 0x0000FF, 0x0001FF, 0x0004FF, 0x000EFF, 0x0020FF, 0x003EFF, 0x006BFF, 0x00ABFF, 0x00FFFF, 0x00FFAB, 0x00FF6B, 0x00FF3E, 0x00FF20, 0x00FF0E, 0x00FF04, 0x00FF01, 0x00FF00, 0x01FF00, 0x04FF00, 0x0EFF00, 0x20FF00, 0x3EFF00, 0x6BFF00, 0xABFF00, 0xFFFF00, 0xFFAB00, 0xFF6B00, 0xFF3E00, 0xFF2000, 0xFF0E00, 0xFF0400, 0xFF0100, };
uint8_t j = 0;

uint8_t led_case = 5;
uint8_t led_pos[6] = {0, 0, 0, 10, 10, 0};
uint16_t sf_Timer;

const int IntPin = A3; /* Definition of the interrupt pin. You can change according to your board */
//Class initialization with the I2C addresses
i2cEncoderLibV2 Encoder(0b1000000); /* A6 is soldered */
LEDRing LEDRing1(0x5A);

void UpdateRing(uint8_t value);
void timer_callback(void);
uint8_t CheckRingBorder(uint8_t in);

//Callback when the CVAL is incremented
void encoder_change(i2cEncoderLibV2* obj) {
  if (led_case == 5)
    return;

  led_pos[led_case] = obj->readCounterByte();
  if (led_case < 3)
    UpdateRing(led_pos[led_case]);

  Serial.print("Change: ");
  Serial.print(led_case);
  Serial.print(" val: ");
  Serial.println(led_pos[led_case]);

}

//Callback when the encoder is released
// it change also the led RGB effect
void encoder_released(i2cEncoderLibV2* obj) {
  Serial.println("Encoder is released");
  led_case++;
  if (led_case > 5)
    led_case = 0;

  switch (led_case) {
    case 0:
      obj->writeCounter((int32_t)led_pos[led_case]);
      obj->writeRGBCode(0xFF0000);
      break;

    case 1:
      obj->writeCounter((int32_t)led_pos[led_case]);
      Encoder.writeRGBCode(0x00FF00);
      break;

    case 2:
      Encoder.writeRGBCode(0x0000FF);
      obj->writeCounter((int32_t)led_pos[led_case]);
      break;

    case 3:
      Timer1.start();
      Encoder.writeMax((int32_t) 100); /* Set the maximum threshold*/
      Encoder.writeMin((int32_t) 0); /* Set the minimum threshold */
      Encoder.writeStep((int32_t) 1); /* Set the step to 1*/
      Encoder.writeRGBCode(0xFF00FF);
      obj->writeCounter((int32_t)led_pos[led_case]);
      break;

    case 4:
      Encoder.writeRGBCode(0x086050);
      obj->writeCounter((int32_t)led_pos[led_case]);
      break;

    case 5:
      Timer1.stop();
      Encoder.writeRGBCode(0);
      for (uint8_t i = 0; i < 48; i++) {
        LEDRing1.LEDRing_Set_RGB(i, 0);
        delay(5);
      }

      led_case = 0;
      UpdateRing(led_pos[led_case]);
      led_case = 1;
      UpdateRing(led_pos[led_case]);
      led_case = 2;
      UpdateRing(led_pos[led_case]);
      led_case = 5;

      Encoder.writeMax((int32_t) 47); /* Set the maximum threshold*/
      Encoder.writeMin((int32_t) 0); /* Set the minimum threshold */
      Encoder.writeStep((int32_t) 2); /* Set the step to 1*/

      break;
  }
}

//Update the led Ring with the encoder position
void UpdateRing(uint8_t value) {

  switch (led_case) {
    case 0:
      for (uint8_t i = 0; i < 48; i++)
        LEDRing1.LEDRing_Set_RED(i, 0);

      LEDRing1.LEDRing_Set_RED(CheckRingBorder(value + 2), 0x03);
      LEDRing1.LEDRing_Set_RED(CheckRingBorder(value + 1), 0x10);
      LEDRing1.LEDRing_Set_RED(CheckRingBorder(value), 0xff);
      LEDRing1.LEDRing_Set_RED(CheckRingBorder(value - 1), 0x10);
      LEDRing1.LEDRing_Set_RED(CheckRingBorder(value - 2), 0x03);
      break;

    case 1:
      for (uint8_t i = 0; i < 48; i++)
        LEDRing1.LEDRing_Set_GREEN(i, 0);
      LEDRing1.LEDRing_Set_GREEN(CheckRingBorder(value + 2), 0x03);
      LEDRing1.LEDRing_Set_GREEN(CheckRingBorder(value + 1), 0x10);
      LEDRing1.LEDRing_Set_GREEN(CheckRingBorder(value), 0xff);
      LEDRing1.LEDRing_Set_GREEN(CheckRingBorder(value - 1), 0x10);
      LEDRing1.LEDRing_Set_GREEN(CheckRingBorder(value - 2), 0x03);
      break;

    case 2:
      for (uint8_t i = 0; i < 48; i++)
        LEDRing1.LEDRing_Set_BLUE(i, 0);

      LEDRing1.LEDRing_Set_BLUE(CheckRingBorder(value + 2), 0x03);
      LEDRing1.LEDRing_Set_BLUE(CheckRingBorder(value + 1), 0x10);
      LEDRing1.LEDRing_Set_BLUE(CheckRingBorder(value), 0xff);
      LEDRing1.LEDRing_Set_BLUE(CheckRingBorder(value - 1), 0x10);
      LEDRing1.LEDRing_Set_BLUE(CheckRingBorder(value - 2), 0x03);
      break;
  }

}

// Callback when the Timer1 expire, used for make the LED pattern roatating
void timer_callback(void) {
  sf_Timer++;
}

// Small function for wrap the led position between 0 and 47.
uint8_t CheckRingBorder(uint8_t in) {

  if ((int8_t)in > 47) {
    return ((int8_t)in - 48);
  }
  if ((int8_t)in < 0) {

    return ( 48 + (int8_t)in);
  }
  return in;
}

void setup(void) {
  pinMode(IntPin, INPUT);
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timer_callback);

  Wire.begin();
  Wire.setClock(500000);
  Serial.begin(115200);
  Serial.println("**** I2C Encoder + LED Ring example ****");

  Encoder.reset();
  Encoder.begin(i2cEncoderLibV2::INT_DATA | i2cEncoderLibV2::WRAP_ENABLE
                | i2cEncoderLibV2::DIRE_LEFT | i2cEncoderLibV2::IPUP_ENABLE
                | i2cEncoderLibV2::RMOD_X2 | i2cEncoderLibV2::RGB_ENCODER);

// The i2c encoder is set to RMOD_X2, in this way you will see the LED dot moving meawhile the encoder is in the middle of a step.

  Encoder.writeCounter((int32_t) 0); /* Reset the counter value */
  Encoder.writeMax((int32_t) 47); /* Set the maximum threshold*/
  Encoder.writeMin((int32_t) 0); /* Set the minimum threshold */
  Encoder.writeStep((int32_t) 1); /* Set the step to 1*/
  Encoder.writeAntibouncingPeriod(1); /* Set an anti-bouncing of 200ms */
  Encoder.writeDoublePushPeriod(0); /*Set a period for the double push of 500ms */

  // Definition of the encoder events
  Encoder.onChange = encoder_change; 
  Encoder.onButtonRelease = encoder_released;


  /* Enable the I2C Encoder V2 interrupts according to the previus attached callback */
  Encoder.autoconfigInterrupt();
  LEDRing1.LEDRing_Reset();
  delay(20);

  LEDRing1.LEDRing_EnableAllOutput();
  LEDRing1.LEDRing_Configuration(0x01);
  LEDRing1.LEDRing_GlobalCurrent(0xff);
  LEDRing1.LEDRing_PULLUP(0b110);
  LEDRing1.LEDRing_PULLDOWN(0b110);
  LEDRing1.LEDRing_PWM_MODE();
  
  randomSeed(analogRead(0));

 // Showing some random pattern
  for (uint8_t i = 0; i < 100; i++) {
    LEDRing1.LEDRing_Set_RGB(random(47), random(0xFFFFFF));
    delay(20);
  }
  for (uint8_t i = 0; i < 48; i++) {
    LEDRing1.LEDRing_Set_RGB(i, fade2_table[i]);
    delay(10);
  }
  for (uint8_t i = 0; i < 48; i++) {
    LEDRing1.LEDRing_Set_RGB(i, 0);
    delay(5);
  }
}

void loop() {
  uint8_t temp;
  //Check the Encoder Status
  if (digitalRead(IntPin) == LOW) {
    Encoder.updateStatus();
  }

  // Make the LED pattern rotating, it's used the timer1 for the temporization.
  if (sf_Timer > led_pos[led_case]) {
    sf_Timer = 0;

    switch (led_case) {
      case 3:
        temp = j++;
        if (j >= 48)
          j = 0;
        for (uint8_t i = 0; i < 48; i++) {
          LEDRing1.LEDRing_Set_RGB(i, fade1_table[temp]);
          temp++;
          if (temp >= 48)
            temp = 0;
        }
        break;

      case 4:
        temp = j++;
        if (j >= 48)
          j = 0;
        for (uint8_t i = 0; i < 48; i++) {
          LEDRing1.LEDRing_Set_RGB(i, fade2_table[temp]);
          temp++;
          if (temp >= 48)
            temp = 0;
        }
        break;
    }
  }
}
