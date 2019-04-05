#include <Wire.h>
#include <I2CNavKey.h>

/*This is a basic example for using the I2C navkey V2
  The counter is set to work between +10 to -10, at every navkey click the counter value is printed on the terminal.
  It's also printet when the push button is pressed, or released or double pushed.


  Connections with Arduino UNO:
  - -> GND
  + -> 5V
  SDA -> A4
  SCL -> A5
  INT -> A3
*/


const int IntPin = A3; /* Definition of the interrupt pin*/
//Class initialization with the I2C addresses
i2cNavKey navkey(0x30); /* For make the address 0x61 only the jumpers A0, A5 and A6 are soldered.*/


void UP_Button_Pressed(void) {
  Serial.println("Button UP Pressed!");
}

void DOWN_Button_Pressed(void) {
  Serial.println("Button DOWN Pressed!");
}

void LEFT_Button_Pressed(void) {
  Serial.println("Button LEFT Pressed!");
}

void RIGHT_Button_Pressed(void) {
  Serial.println("Button RIGHT Pressed!");
}

void CENTRAL_Button_Pressed(void) {
  Serial.println("Button Central Pressed!");
}

void Encoder_Rotate(void) {
  Serial.println(navkey.readCounterInt());
}


void setup(void)
{
  pinMode(IntPin, INPUT);
  Wire.begin();
  Serial.begin(115200);
  Serial.println("**** I2C navkey V2 basic example ****");
  /*
      INT_DATA= The register are considered integer.
      WRAP_DISABLE= The WRAP option is disabled
      DIRE_LEFT= navkey left direction increase the value
      IPUP_ENABLE= INT pin have the pull-up enabled.
      RMOD_X1= navkey configured as X1.
      RGB_navkey= type of navkey is RGB, change to STD_navkey in case you are using a normal rotary navkey.
  */

  navkey.begin(INT_DATA | WRAP_ENABLE | DIRE_LEFT | IPUP_ENABLE);


  //  navkey.begin(INT_DATA | WRAP_DISABLE | DIRE_LEFT | IPUP_ENABLE | RMOD_X1 | STD_navkey); // try also this!
  //  navkey.begin(INT_DATA | WRAP_ENABLE | DIRE_LEFT | IPUP_ENABLE | RMOD_X1 | RGB_navkey);  // try also this!

  navkey.writeCounter((int32_t)0); /* Reset the counter value */
  navkey.writeMax((int32_t)10); /* Set the maximum threshold*/
  navkey.writeMin((int32_t) - 10); /* Set the minimum threshold */
  navkey.writeStep((int32_t)1); /* Set the step to 1*/

  navkey.writeDoublePushPeriod(0);  /*Set a period for the double push of 500ms */
  
  navkey.onUpPush=UP_Button_Pressed;
  navkey.onDownPush=DOWN_Button_Pressed;
  navkey.onRightPush=RIGHT_Button_Pressed;
  navkey.onLeftPush=LEFT_Button_Pressed;
  navkey.onCentralPush=CENTRAL_Button_Pressed;
  navkey.onChange=Encoder_Rotate;
  navkey.autoconfigInterrupt(); /* Enable all the interrupt */
  
  
  
}

void loop() {
  uint8_t enc_cnt;
  if (digitalRead(IntPin) == LOW) {
    navkey.updateStatus();
  }
}