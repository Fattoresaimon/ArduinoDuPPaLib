#include <Wire.h>
#include <i2cEncoderLibV2.h>

/*In this example a standard encoder is used.
   The GP pins are configured as analog and
   the ADC vale is printed on the serial port
   as well as the counter value


  Connections with Arduino UNO:
  - -> GND
  + -> 5V
  SDA -> A4
  SCL -> A5
  INT -> A3
*/
unsigned long previousMillis = 0;
const long interval = 1000;           // interval between ADC reading of the GP pins

const int IntPin = A3; // Change according to your board configuration

//Class initialization with the I2C addresses
i2cEncoderLibV2 STDEncoder(0b0001000); //Normal rotary encoder address, A3 is soldered


uint8_t encoder_status, i;
void setup(void)
{
  // Reset the two encoder
  Wire.begin();


  // Initialize the GPIO and the display
  pinMode(IntPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  //Initialize the Serial port
  Serial.begin(115200);
  Serial.print("Encoder Test!!\n");


  //Configure the Standard Encoder
  STDEncoder.reset();
  STDEncoder.begin(i2cEncoderLibV2::INT_DATA | i2cEncoderLibV2::WRAP_DISABLE | i2cEncoderLibV2::DIRE_LEFT | i2cEncoderLibV2::IPUP_ENABLE | i2cEncoderLibV2::RMOD_X1 | i2cEncoderLibV2::STD_ENCODER);
  STDEncoder.writeGP1conf(i2cEncoderLibV2::GP_AN | i2cEncoderLibV2::GP_PULL_EN | i2cEncoderLibV2::GP_INT_DI);  // Configure the GP pins in analog mode
  STDEncoder.writeGP2conf(i2cEncoderLibV2::GP_AN | i2cEncoderLibV2::GP_PULL_EN | i2cEncoderLibV2::GP_INT_DI); // Configure the GP pins in analog mode
  STDEncoder.writeGP3conf(i2cEncoderLibV2::GP_AN | i2cEncoderLibV2::GP_PULL_EN | i2cEncoderLibV2::GP_INT_DI); // Configure the GP pins in analog mode
  STDEncoder.writeCounter((int32_t) 0);
  STDEncoder.writeMax((int32_t) 10);
  STDEncoder.writeMin((int32_t) 0);
  STDEncoder.writeStep((int32_t) 1);
  STDEncoder.writeInterruptConfig(i2cEncoderLibV2::INT_2 | i2cEncoderLibV2::RINC | i2cEncoderLibV2::RDEC | i2cEncoderLibV2::RMAX | i2cEncoderLibV2::RMIN); //Enable all the interrupts
  STDEncoder.writeAntibouncingPeriod(20);  /* Set an anti-bouncing of 200ms */
  STDEncoder.updateStatus();

}

void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    Serial.print("GP1: ");
    Serial.print(STDEncoder.readGP1()); //read the analog value of the GP1
    Serial.print("\tGP2: ");
    Serial.print(STDEncoder.readGP2()); //read the analog value of the GP2
    Serial.print("\tGP3: ");
    Serial.println(STDEncoder.readGP3()); //read the analog value of the GP3

  }


  if (digitalRead(IntPin) == LOW) { //Check for the interrupt

    if (STDEncoder.updateStatus()) { //Check if the normal encoder is moved

      //Print the counter value of the normal encoder
      Serial.print("Encoder: ");
      Serial.println(STDEncoder.readCounterInt());
    }
  }
}
