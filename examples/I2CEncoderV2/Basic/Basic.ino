#include <Wire.h>
#include <i2cEncoderLibV2.h>

/*This is a basic example for using the I2C Encoder V2
  The counter is set to work between +10 to -10, at every encoder click the counter value is printed on the terminal.
  It's also printet when the push button is pressed, or released or double pushed.


  Connections with Arduino UNO:
  - -> GND
  + -> 5V
  SDA -> A4
  SCL -> A5
  INT -> A3
*/

const int IntPin = A3; /* Definition of the interrupt pin. You can change according to your board /*
  //Class initialization with the I2C addresses*/
i2cEncoderLibV2 Encoder(0x01); /* A0 is soldered */

void setup(void) {
  pinMode(IntPin, INPUT);
  Wire.begin();
  Serial.begin(115200);
  Serial.println("**** I2C Encoder V2 basic example ****");
  /*
    INT_DATA= The register are considered integer.
    WRAP_DISABLE= The WRAP option is disabled
    DIRE_LEFT= Encoder left direction increase the value
    IPUP_ENABLE= INT pin have the pull-up enabled.
    RMOD_X1= Encoder configured as X1.
    RGB_ENCODER= type of encoder is RGB, change to STD_ENCODER in case you are using a normal rotary encoder.
  */
  Encoder.reset();
  Encoder.begin(
    i2cEncoderLibV2::INT_DATA | i2cEncoderLibV2::WRAP_DISABLE
    | i2cEncoderLibV2::DIRE_LEFT | i2cEncoderLibV2::IPUP_ENABLE
    | i2cEncoderLibV2::RMOD_X1 | i2cEncoderLibV2::RGB_ENCODER);
  
  // Use this in case of standard encoder!
  //  Encoder.begin(i2cEncoderLibV2::INT_DATA | i2cEncoderLibV2::WRAP_DISABLE | i2cEncoderLibV2::DIRE_LEFT | i2cEncoderLibV2::IPUP_ENABLE | i2cEncoderLibV2::RMOD_X1 | i2cEncoderLibV2::STD_ENCODER); 
  
  // try also this!
  //  Encoder.begin(i2cEncoderLibV2::INT_DATA | i2cEncoderLibV2::WRAP_ENABLE | i2cEncoderLibV2::DIRE_LEFT | i2cEncoderLibV2::IPUP_ENABLE | i2cEncoderLibV2::RMOD_X1 | i2cEncoderLibV2::RGB_ENCODER);  

  Encoder.writeCounter((int32_t) 0); /* Reset the counter value */
  Encoder.writeMax((int32_t) 10); /* Set the maximum threshold*/
  Encoder.writeMin((int32_t) - 10); /* Set the minimum threshold */
  Encoder.writeStep((int32_t) 1); /* Set the step to 1*/
  Encoder.writeInterruptConfig(0xff); /* Enable all the interrupt */
  Encoder.writeAntibouncingPeriod(20); /* Set an anti-bouncing of 200ms */
  Encoder.writeDoublePushPeriod(50); /*Set a period for the double push of 500ms */
}

void loop() {

  if (digitalRead(IntPin) == LOW) {
    if (Encoder.updateStatus()) {
      if (Encoder.readStatus(i2cEncoderLibV2::RINC)) {
        Serial.print("Increment: ");
        Serial.println(Encoder.readCounterByte());

        /* Write here your code */

      }
      if (Encoder.readStatus(i2cEncoderLibV2::RDEC)) {
        Serial.print("Decrement: ");
        Serial.println(Encoder.readCounterByte());

        /* Write here your code */

      }

      if (Encoder.readStatus(i2cEncoderLibV2::RMAX)) {
        Serial.print("Maximum threshold: ");
        Serial.println(Encoder.readCounterByte());

        /* Write here your code */

      }

      if (Encoder.readStatus(i2cEncoderLibV2::RMIN)) {
        Serial.print("Minimum threshold: ");
        Serial.println(Encoder.readCounterByte());

        /* Write here your code */

      }

      if (Encoder.readStatus(i2cEncoderLibV2::PUSHR)) {
        Serial.println("Push button Released");

        /* Write here your code */

      }

      if (Encoder.readStatus(i2cEncoderLibV2::PUSHP)) {
        Serial.println("Push button Pressed");

        /* Write here your code */

      }

      if (Encoder.readStatus(i2cEncoderLibV2::PUSHD)) {
        Serial.println("Double push!");

        /* Write here your code */

      }
    }
  }
}
