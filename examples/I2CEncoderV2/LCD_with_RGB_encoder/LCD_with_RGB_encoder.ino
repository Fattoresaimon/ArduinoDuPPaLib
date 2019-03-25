#include <Wire.h>
#include <i2cEncoderLibV2.h>
#include <LiquidCrystal.h>

/*In this example, i want my variable counter between -10 and 10.
  When it reaches the limit the LED will blink red in case of minimum and will blink green when it reaches the maximum.
  The INT pin is readed in polling mode

  Connections with Arduino UNO:
  - -> GND
  + -> 5V
  SDA -> A4
  SCL -> A5
  INT -> 12
*/
unsigned long previousMillis = 0;
const long interval = 100;
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int lcd_key = 0;
int adc_key_in = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
#define ENCODER_N 2

const int IntPin = A3;
//Class initialization with the I2C addresses
i2cEncoderLibV2 RGBEncoder(0b1100001); /* For make the address 0x61 only the jumpers A0, A5 and A6 are soldered.*/

// read the buttons
int read_LCD_buttons() {
  adc_key_in = analogRead(0);      // read the value from the sensor
  // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  // we add approx 50 to those values and check to see if we are close
  if (adc_key_in > 1000)
    return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
  // For V1.1 us this threshold
  if (adc_key_in < 50)
    return btnRIGHT;
  if (adc_key_in < 250)
    return btnUP;
  if (adc_key_in < 450)
    return btnDOWN;
  if (adc_key_in < 650)
    return btnLEFT;
  if (adc_key_in < 850)
    return btnSELECT;
  return btnNONE;  // when all others fail, return this...
}

uint8_t encoder_status, i;
void setup(void) {
  Wire.begin();
  Wire.setClock(400000);
  RGBEncoder.reset();

  delay(1000);
  pinMode(IntPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  lcd.begin(16, 2);              // start the library
  lcd.setCursor(0, 0);

  Serial.begin(115200);
  Serial.print("Encoder Test!!\n");

  RGBEncoder.begin(
    i2cEncoderLibV2::FLOAT_DATA | i2cEncoderLibV2::WRAP_DISABLE | i2cEncoderLibV2::DIRE_LEFT | i2cEncoderLibV2::IPUP_ENABLE | i2cEncoderLibV2::RMOD_X1
    | i2cEncoderLibV2::RGB_ENCODER);

  RGBEncoder.writeCounter((float) 0);
  RGBEncoder.writeMax((float) 10);
  RGBEncoder.writeMin((float) - 10);
  RGBEncoder.writeStep((float) 0.5);
  RGBEncoder.writeInterruptConfig(0xff);
  RGBEncoder.writeDoublePushPeriod(50);
  RGBEncoder.writeFadeRGB(1);
  RGBEncoder.updateStatus();
  RGBEncoder.writeAntibouncingPeriod(25);
  RGBEncoder.writeGP1conf(i2cEncoderLibV2::GP_PWM | i2cEncoderLibV2::GP_PULL_EN | i2cEncoderLibV2::GP_INT_DI);
  RGBEncoder.writeGP2conf(i2cEncoderLibV2::GP_PWM | i2cEncoderLibV2::GP_PULL_EN | i2cEncoderLibV2::GP_INT_DI);
  RGBEncoder.writeFadeRGB(2);

}

void loop() {

  if (digitalRead(IntPin) == LOW) {
    Serial.println("Pin LOW ");

    ///////////////////////////////////////////////////////////////////
    if (RGBEncoder.updateStatus()) {

      if (RGBEncoder.readStatus(i2cEncoderLibV2::RINC)) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Inc: ");
        lcd.print(RGBEncoder.readCounterFloat(), 1);
        lcd.print("   ");
        if ((RGBEncoder.readStatus(i2cEncoderLibV2::RMIN) == 0)
            && (RGBEncoder.readStatus(i2cEncoderLibV2::RMAX) == 0))
          RGBEncoder.writeLEDG(0xFF);
      }
      if (RGBEncoder.readStatus(i2cEncoderLibV2::RDEC)) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Dec: ");
        lcd.print(RGBEncoder.readCounterFloat(), 1);
        lcd.print("   ");
        if ((RGBEncoder.readStatus(i2cEncoderLibV2::RMIN) == 0)
            && (RGBEncoder.readStatus(i2cEncoderLibV2::RMAX) == 0))
          RGBEncoder.writeLEDG(0xFF);
      }

      if (RGBEncoder.readStatus(i2cEncoderLibV2::RMAX)) {

        lcd.setCursor(0, 1);
        lcd.print("Counter MAX     ");
        RGBEncoder.writeLEDR(0xFF);
      }

      if (RGBEncoder.readStatus(i2cEncoderLibV2::RMIN)) {

        lcd.setCursor(0, 1);
        lcd.print("Counter MIN    ");
        RGBEncoder.writeLEDR(0xFF);
      }

      if (RGBEncoder.readStatus(i2cEncoderLibV2::PUSHR)) {

        lcd.setCursor(0, 1);
        lcd.print("B. Released   ");
        RGBEncoder.writeRGBCode(0x0000FF);
      }

      if (RGBEncoder.readStatus(i2cEncoderLibV2::PUSHP)) {

        lcd.setCursor(0, 1);
        lcd.print("B. Pushed    ");
        RGBEncoder.writeRGBCode(0x0000FF);
      }

      if (RGBEncoder.readStatus(i2cEncoderLibV2::PUSHD)) {

        lcd.setCursor(0, 1);
        lcd.print("Double Push   ");
        RGBEncoder.writeRGBCode(0x800080);
      }

      if (RGBEncoder.readInt2(i2cEncoderLibV2::FADE_INT)) { //Check if the fade proccess finished, if yes turn off the LEDs
        RGBEncoder.writeRGBCode(0x0);
      }

    }
  }
}
