#include <Wire.h>
#include <i2cEncoderLibV2.h>

/*In this example there are 9 I2C Encoder V2 boards with the RGB LED connected in a matrix 3x3
  There is also the Arduino Serial KeyPad Shield attached.

  Every time of one encoder is moved it's value is showed on the display
  In this example the data type are float

  Connections with Arduino UNO:
  - -> GND
  + -> 5V
  SDA -> A4
  SCL -> A5
  INT -> A3
*/

#define ENCODER_N 9 //Number limit of the encoder

const int IntPin = A3; // INT pins, change according to your board

//Class initialization with the I2C addresses
i2cEncoderLibV2 RGBEncoder[ENCODER_N] = { i2cEncoderLibV2(0x40),
                                          i2cEncoderLibV2(0x20), i2cEncoderLibV2(0x60), i2cEncoderLibV2(0x10),
                                          i2cEncoderLibV2(0x50), i2cEncoderLibV2(0x30), i2cEncoderLibV2(0x70),
                                          i2cEncoderLibV2(0x04), i2cEncoderLibV2(0x44),
                                        };

uint8_t encoder_status, i;

void encoder_rotated(i2cEncoderLibV2* obj) {
  if (obj->readStatus(i2cEncoderLibV2::RINC))
    Serial.print("Increment ");
  else
    Serial.print("Decrement ");
  Serial.print(obj->id);
  Serial.print(": ");
  Serial.println(obj->readCounterInt());
  obj->writeRGBCode(0x00FF00);
}

void encoder_click(i2cEncoderLibV2* obj) {
  Serial.print("Push: ");
  Serial.println(obj->id);
  obj->writeRGBCode(0x0000FF);
}

void encoder_thresholds(i2cEncoderLibV2* obj) {
  if (obj->readStatus(i2cEncoderLibV2::RMAX))
    Serial.print("Max: ");
  else
    Serial.print("Min: ");
  Serial.println(obj->id);
  obj->writeRGBCode(0xFF0000);
}

void encoder_fade(i2cEncoderLibV2* obj) {
  obj->writeRGBCode(0x000000);
}

void setup(void) {
  uint8_t enc_cnt;

  Wire.begin();
  //Reset of all the encoder ìs
  for (enc_cnt = 0; enc_cnt < ENCODER_N; enc_cnt++) {
    RGBEncoder[enc_cnt].reset();
  }

  pinMode(IntPin, INPUT);

  Serial.begin(115200);
  Serial.print("Encoder Test!!\n");

  // Initialization of the encoders
  for (enc_cnt = 0; enc_cnt < ENCODER_N; enc_cnt++) {
    RGBEncoder[enc_cnt].begin(
      i2cEncoderLibV2::INT_DATA | i2cEncoderLibV2::WRAP_DISABLE
      | i2cEncoderLibV2::DIRE_RIGHT
      | i2cEncoderLibV2::IPUP_ENABLE
      | i2cEncoderLibV2::RMOD_X1
      | i2cEncoderLibV2::RGB_ENCODER);
    RGBEncoder[enc_cnt].writeCounter((int32_t) 0); //Reset of the CVAL register
    RGBEncoder[enc_cnt].writeMax((int32_t) 50); //Set the maximum threshold to 50
    RGBEncoder[enc_cnt].writeMin((int32_t) 0); //Set the minimum threshold to 0
    RGBEncoder[enc_cnt].writeStep((int32_t) 1); //The step at every encoder click is 1
    RGBEncoder[enc_cnt].writeRGBCode(0);
    RGBEncoder[enc_cnt].writeFadeRGB(3); //Fade enabled with 3ms step
    RGBEncoder[enc_cnt].writeAntibouncingPeriod(25); //250ms of debouncing
    RGBEncoder[enc_cnt].writeDoublePushPeriod(0); //Set the double push period to 500ms

    /* Configure the events */
    RGBEncoder[enc_cnt].onChange = encoder_rotated;
    RGBEncoder[enc_cnt].onButtonRelease = encoder_click;
    RGBEncoder[enc_cnt].onMinMax = encoder_thresholds;
    RGBEncoder[enc_cnt].onFadeProcess = encoder_fade;

    /* Enable the I2C Encoder V2 interrupts according to the previus attached callback */
    RGBEncoder[enc_cnt].autoconfigInterrupt();
    RGBEncoder[enc_cnt].id = enc_cnt;
  }
}

void loop() {
  uint8_t enc_cnt;

  if (digitalRead(IntPin) == LOW) {
    //Interrupt from the encoders, start to scan the encoder matrix
    for (enc_cnt = 0; enc_cnt < ENCODER_N; enc_cnt++) {
      if (digitalRead(IntPin) == HIGH) { //If the interrupt pin return high, exit from the encoder scan
        break;
      }
      RGBEncoder[enc_cnt].updateStatus();
    }
  }
}
