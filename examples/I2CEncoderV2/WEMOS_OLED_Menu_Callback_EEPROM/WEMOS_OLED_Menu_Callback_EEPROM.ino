#include<Wire.h>
#include <ESP8266WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <i2cEncoderLibV2.h>

/*This is an example of using the I2C Encoder V2 for scrolling a simple menu'.
   For this example is required the WEMOS and the OLED shield.
   There are 4 items: A, B, C, D. Each items has it's own variable.
   With the Encoder is possible to select the items and change it's own variable.
   When the encoder LED is GREEN it's possible to select the item.
   By clicking the encoder the LED became BLUE and it's possible to change the value of the item.
   With a double push the item values is store in the EEPROM of the encoder.
   The LED return GREEN and again it's possbile to select another item. 


  Connections with WEMOS board:
  - -> GND
  + -> 3.3V
  SDA -> D2
  SCL -> D1
  INT -> D0
*/


const int IntPin = 0; /* Definition of the interrupt pin*/
//Class initialization with the I2C addresses
i2cEncoderLibV2 Encoder(0x61); /* For make the address 0x61 only the jumpers A0, A5 and A6 are soldered.*/

#define EEPROM_START_ADD  0x0F

#define OLED_RESET -1
Adafruit_SSD1306 display(OLED_RESET);

uint8_t m_pos = 0;
int8_t val[4] = {0};  /* array where it's possbile to store the 4 value of the items */
int8_t max_val[4] = {10, 5, 20, 11}; /* array where is store the max value of each items. Customize according to your necessity */
int8_t min_val[4] = { -3, 0, 3, -11}; /* array where is store the min value of each items. Customize according to your necessity */

bool insert = false;  /* if it's false is the item value selection, when it's true is the item selection */


void encoder_rotated(i2cEncoderLibV2* obj, SourceInt e) {
  if (insert == false) {
    m_pos = obj->readCounterByte();  /* change the item */
    Serial.println(m_pos);
  }
  if (insert == true) {
    val[m_pos] = obj->readCounterByte(); /* change the item value */
  }
}

void encoder_pushed(i2cEncoderLibV2* obj, SourceInt e) {
  insert = true;

  obj->writeCounter((int32_t)val[m_pos]); /* Reset the counter value */
  obj->writeMax((int32_t)max_val[m_pos] ); /* Set the maximum threshold*/
  obj->writeMin((int32_t)min_val[m_pos]); /* Set the minimum threshold */
  obj->writeStep((int32_t)1); /* Set the step to 1*/
  obj->writeRGBCode(0x0000FF);
  obj->attachInterrupt(encoder_Double_pushed, BUTTON_DOUBLE_PUSH); // Enable the double push interrupt
}

void encoder_Double_pushed(i2cEncoderLibV2* obj, SourceInt e) {
  insert = false;
  obj->writeCounter((int32_t)0); /* Reset the counter value */
  obj->writeMax((int32_t)3); /* Set the maximum threshold*/
  obj->writeMin((int32_t)0); /* Set the minimum threshold */
  obj->writeStep((int32_t)1); /* Set the step to 1*/
  obj->writeRGBCode(0x00ff00);

  obj->writeEEPROM(EEPROM_START_ADD + m_pos, (uint8_t)val[m_pos]);
  obj->deattachInterrupt(BUTTON_DOUBLE_PUSH);  // Disable the double push interrupt

}


void setup(void)
{
  Serial.begin(115200);
  Wire.begin();
  Encoder.reset(); /* Reset the I2C encoder V2 and wait 100ms */

  pinMode(IntPin, INPUT);
  /*
      INT_DATA= The register are considered integer.
      WRAP_ENABLE= The WRAP option is enabled
      DIRE_RIGHT= Encoder right direction increase the value
      IPUP_ENABLE= INT pin have the pull-up enabled.
      RMOD_X1= Encoder configured as X1.
      RGB_ENCODER= type of encoder is RGB, change to STD_ENCODER in case you are using a normal rotary encoder.
  */

  Encoder.begin(INT_DATA | WRAP_ENABLE | DIRE_RIGHT | IPUP_ENABLE | RMOD_X1 | RGB_ENCODER);
  Encoder.writeCounter((int32_t)0); /* Reset the counter value */
  Encoder.writeMax((int32_t)3); /* Set the maximum threshold*/
  Encoder.writeMin((int32_t)0); /* Set the minimum threshold */
  Encoder.writeStep((int32_t)1); /* Set the step to 1*/
  Encoder.writeAntibouncingPeriod(30);  /* Set an anti-bouncing of 300ms */
  Encoder.writeDoublePushPeriod(50);  /*Set a period for the double push of 300ms */
  Encoder.writeFadeRGB(1); /*Set fade to 1ms step */
  Encoder.writeRGBCode(0x00ff00); /* Turn ON the green LED */

  Encoder.attachInterrupt(encoder_rotated, ENCODER_INCREMENT);
  Encoder.attachInterrupt(encoder_rotated, ENCODER_DECREMENT);
  Encoder.attachInterrupt(encoder_pushed, BUTTON_RELEASE);
  Encoder.attachInterrupt(encoder_Double_pushed, BUTTON_DOUBLE_PUSH);
  Encoder.autoconfigInterrupt();




  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  /* initialize the item values to the minimum value */
  /* Initialie also the EEPROM contenet */
  for (uint8_t i = 0; i < 4; i++) {
    
    val[i] = Encoder.readEEPROM(EEPROM_START_ADD + i);

    if ( (val[i] > max_val[i]) || (val[i] < min_val[i])) {
      val[i] = min_val[i];
      Encoder.writeEEPROM(EEPROM_START_ADD + i, (uint8_t)val[i]);
    }
  }

  menu();
}


void loop() {

  if (digitalRead(IntPin) == LOW) {
    if ( Encoder.updateStatus()) {
      menu();
    }
  }
}


/* fuction that print the menu' */
void menu(void) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Menu':");
  display.setCursor(10, 10);
  display.print("A: ");
  display.print(val[0]);
  display.setCursor(10, 20);
  display.print("B: ");
  display.print(val[1]);
  display.setCursor(10, 30);
  display.print("C: ");
  display.print(val[2]);
  display.setCursor(10, 40);
  display.print("D: ");
  display.print(val[3]);

  display.setCursor(0, 10 + (m_pos * 10));
  display.print(">");

  display.display();


}
