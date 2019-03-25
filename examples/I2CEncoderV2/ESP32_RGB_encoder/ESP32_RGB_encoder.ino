#include <Wire.h>
#include <i2cEncoderLibV2.h>

/*This is a basic example for using the I2C Encoder V2
  The counter is set to work between +10 to -10, at every encoder click the counter value is printed on the terminal.
  It's also printet when the push button is released.
  When the encoder is turned the led turn green
  When the encoder reach the max or min the led turn red
  When the encoder is pushed, the led turn blue

  Connections with ESP-32 Dev Kit:
  - -> GND
  + -> 3.3V
  SDA -> 21
  SCL -> 22
  INT -> 23
*/

const int IntPin = 23; /* Definition of the interrupt pin, change according to your configuration*/
//Class initialization with the I2C addresses
i2cEncoderLibV2 Encoder(0b1100001); /* For make the address 0x61 only the jumpers A0, A5 and A6 are soldered.*/

//Callback when the encoder is rotated
void encoder_rotated(i2cEncoderLibV2* obj) {
  if ( obj->readStatus( i2cEncoderLibV2::RINC))
    Serial.print("Increment: ");
  else
    Serial.print("Decrement: ");
  Serial.println( obj->readCounterInt());
  obj->writeRGBCode(0x00FF00);
}


//Callback when the encoder is pushed
void encoder_click(i2cEncoderLibV2* obj) {
  Serial.println("Push: ");
  obj->writeRGBCode(0x0000FF);
}


//Callback when the encoder reach the max or min
void encoder_thresholds(i2cEncoderLibV2* obj) {
  if ( obj->readStatus( i2cEncoderLibV2::RMAX))
    Serial.println("Max!");
  else
    Serial.println("Min!");

  obj->writeRGBCode(0xFF0000);
}


//Callback when the fading process finish and set the RGB led off
void encoder_fade(i2cEncoderLibV2* obj) {
  obj->writeRGBCode(0x000000);
}


void setup(void)
{
  pinMode(IntPin, INPUT);

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
  Wire.begin();
  Encoder.reset();

  Encoder.begin(i2cEncoderLibV2::INT_DATA |i2cEncoderLibV2:: WRAP_DISABLE | i2cEncoderLibV2::DIRE_LEFT | i2cEncoderLibV2::IPUP_ENABLE | i2cEncoderLibV2::RMOD_X1 | i2cEncoderLibV2::RGB_ENCODER);
  //  Encoder.begin(i2cEncoderLibV2::INT_DATA | i2cEncoderLibV2::WRAP_DISABLE | i2cEncoderLibV2::DIRE_LEFT | i2cEncoderLibV2::IPUP_ENABLE | i2cEncoderLibV2::RMOD_X1 | i2cEncoderLibV2::STD_ENCODER); // try also this!
  //  Encoder.begin(i2cEncoderLibV2::INT_DATA | i2cEncoderLibV2::WRAP_ENABLE | i2cEncoderLibV2::DIRE_LEFT | i2cEncoderLibV2::IPUP_ENABLE | i2cEncoderLibV2::RMOD_X1 | i2cEncoderLibV2::RGB_ENCODER);  // try also this!

  Encoder.writeCounter((int32_t)0); /* Reset the counter value */
  Encoder.writeMax((int32_t)10); /* Set the maximum threshold*/
  Encoder.writeMin((int32_t) - 10); /* Set the minimum threshold */
  Encoder.writeStep((int32_t)1); /* Set the step to 1*/
  /* Configure the events */
  Encoder.onChange = encoder_rotated;
  Encoder.onButtonRelease = encoder_click;
  Encoder.onMinMax = encoder_thresholds;
  Encoder.onFadeProcess = encoder_fade;

  /* Enable the I2C Encoder V2 interrupts according to the previus attached callback */
  Encoder.autoconfigInterrupt();

  Encoder.writeAntibouncingPeriod(20);  /* Set an anti-bouncing of 200ms */

  /* blink the RGB LED */
  Encoder.writeRGBCode(0xFF0000);
  delay(250);
  Encoder.writeRGBCode(0x00FF00);
  delay(250);
  Encoder.writeRGBCode(0x0000FF);
  delay(250);
  Encoder.writeRGBCode(0x000000);

  Encoder.writeFadeRGB(3); //Fade enabled with 3ms step

}

void loop() {

  /* Waith when the INT pin goes low */
  if (digitalRead(IntPin) == LOW) {
    /* Check the status of the encoder and call the callback */
    Encoder.updateStatus();
  }
}