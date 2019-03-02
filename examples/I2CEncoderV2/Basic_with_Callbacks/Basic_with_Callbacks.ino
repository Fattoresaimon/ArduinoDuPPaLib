#include <Wire.h>
#include <i2cEncoderLibV2.h>

/*This is a basic example for using the I2C Encoder V2
  The counter is set to work between +10 to -10, at every encoder click the counter value is printed on the terminal.
  It's also printet when the push button is pressed, or released or double pushed.
  The callback are used instead of the switch case

  Connections with Arduino UNO:
  - -> GND
  + -> 5V
  SDA -> A4
  SCL -> A5
  INT -> A3
*/

const int IntPin = A3; /* Definition of the interrupt pin. You can change according to your board */
//Class initialization with the I2C addresses
i2cEncoderLibV2 Encoder(0x61); /* A0 is soldered */


//Callback when the CVAL is incremented
void encoder_increment(i2cEncoderLibV2* obj, SourceInt e) {
  Serial.print("Increment: ");
  Serial.println( Encoder.readCounterByte());
}



//Callback when the CVAL is decremented
void encoder_decrement(i2cEncoderLibV2* obj, SourceInt e) {
  Serial.print("Decrement: ");
  Serial.println( Encoder.readCounterByte());
}



//Callback when CVAL reach MAX
void encoder_max(i2cEncoderLibV2* obj, SourceInt e) {
  Serial.print("Maximum threshold: ");
  Serial.println( Encoder.readCounterByte());
}




//Callback when CVAL reach MIN
void encoder_min(i2cEncoderLibV2* obj, SourceInt e) {
  Serial.print("Minimum threshold: ");
  Serial.println( Encoder.readCounterByte());
}




//Callback when the encoder is pushed
void encoder_push(i2cEncoderLibV2* obj, SourceInt e) {
  Serial.println("Encoder is pushed!");
}



//Callback when the encoder is released
void encoder_released(i2cEncoderLibV2* obj, SourceInt e) {
  Serial.println("Encoder is released");
}



//Callback when the encoder is double pushed
void encoder_double_push(i2cEncoderLibV2* obj, SourceInt e) {
  Serial.println("Encoder is double pushed!");
}




void setup(void)
{
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
  Encoder.begin(INT_DATA | WRAP_DISABLE | DIRE_LEFT | IPUP_ENABLE | RMOD_X1 | RGB_ENCODER);
  //  Encoder.begin(INT_DATA | WRAP_DISABLE | DIRE_LEFT | IPUP_ENABLE | RMOD_X1 | STD_ENCODER); // try also this!
  //  Encoder.begin(INT_DATA | WRAP_ENABLE | DIRE_LEFT | IPUP_ENABLE | RMOD_X1 | RGB_ENCODER);  // try also this!

  Encoder.writeCounter((int32_t)0); /* Reset the counter value */
  Encoder.writeMax((int32_t)10); /* Set the maximum threshold*/
  Encoder.writeMin((int32_t) - 10); /* Set the minimum threshold */
  Encoder.writeStep((int32_t)1); /* Set the step to 1*/
  Encoder.writeAntibouncingPeriod(20);  /* Set an anti-bouncing of 200ms */
  Encoder.writeDoublePushPeriod(50);  /*Set a period for the double push of 500ms */

  Encoder.attachInterrupt(encoder_increment, ENCODER_INCREMENT);
  Encoder.attachInterrupt(encoder_decrement, ENCODER_DECREMENT);
  Encoder.attachInterrupt(encoder_max, ENCODER_MAX);
  Encoder.attachInterrupt(encoder_min, ENCODER_MIN);
  Encoder.attachInterrupt(encoder_push, BUTTON_PUSH);
  Encoder.attachInterrupt(encoder_released, BUTTON_RELEASE);
  Encoder.attachInterrupt(encoder_double_push, BUTTON_DOUBLE_PUSH);
  Encoder.autoconfigInterrupt(); /* Enable all the attached interrupt */

}

void loop() {
  if (digitalRead(IntPin) == LOW) {
    /* Check the status of the encoder and call the callback */
    Encoder.updateStatus();
  }
}
