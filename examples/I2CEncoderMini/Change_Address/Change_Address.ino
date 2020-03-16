#include <Wire.h>
#include <i2cEncoderMiniLib.h>

/* This example is used for change the address of the I2C Encoder Mini
  Connections with Arduino UNO:
  - -> GND
  + -> 5V
  SDA -> A4
  SCL -> A5

  The COM boudrate is 115200.
  You can use the command 'S' for find the I2C encoder mini on the I2C bus.
  Or you can write directly the address in decimal or hexadecimal.
  After after you need to write the new address in the same way.
  Example:

    **** I2C Encoder Address changer! ****
    All the devices must have different address!
    Avaiable commands:
      S: for searching the I2C encoder mini
      0xXX or XXX: Address of the target device in hex or decimal

  S 
    
    Scanning....
    I2C Encoder mini found at 33 ( 0x21 ) !!
    
    
    All the devices must have different address!
    Avaiable commands:
      S: for searching the I2C encoder mini
      0xXX or XXX: Address of the target device in hex or decimal

  0x21
    
    Insert the desired address in hex or decimal: 

  24
    
    Changing address from 33 ( 0x21 ) to 24 ( 0x18 )
    Address Changed!

*/

void ChangeAddress(uint8_t add);
int8_t NewAdd(void);
int8_t CommadParser(void);
int8_t NumberParser(char buff[]);
bool CommandRead(void);
void Search(void);
void PrintHEX(uint8_t i);

i2cEncoderMiniLib Encoder(0x20);


char c_buff[5] = {0};
uint8_t c_buff_cnt = 0;

int8_t t_add;


void setup(void) {
  Wire.begin();
  Serial.begin(115200);
  Serial.println("\n\n**** I2C Encoder Address changer! ****");
}

void loop() {
  Serial.println("All the devices must have different address!");
  Serial.println("Avaiable commands:");
  Serial.println("  S: for searching the I2C encoder mini");
  Serial.println("  0xXX or XXX: Address of the target device in hex or decimal\n");

  t_add = CommadParser();
  if (t_add <= 0) {
    switch (t_add) {
      case -1:
        Serial.println("Wrong address range!");
        break;

      case -2:
        Serial.println("Wrong command!");
        break;

      case -3:
        Search();
        break;
    }
  } else {
    Encoder.address = t_add;
    if (Encoder.readIDCode() == 0x39) {
      Serial.println("Insert the desired address in hex or decimal: ");
      t_add = NewAdd();
      if (t_add <= 0) {
        Serial.println("Incorrect Address!");
      } else {
        ChangeAddress(t_add);
      }
    } else {
      Serial.println("No device at that address!");
    }
  }
  Serial.print("\n\n");
}


void ChangeAddress(uint8_t add) {
  Serial.print("Changing address from ");
  Serial.print(Encoder.address);
  Serial.print(" ( ");
  PrintHEX(Encoder.address);
  Serial.print(" ) to ");
  Serial.print(add);
  Serial.print(" ( ");
  PrintHEX(add);
  Serial.println(" )");
  Encoder.ChangeI2CAddress(add);
  // delay(1000);
  Encoder.reset();
  Encoder.address = add;
  if (Encoder.readIDCode() == 0x39) {
    Serial.println("Address Changed!");
  } else {
    Serial.println("Error in changing address!");
  }
}


int8_t NewAdd(void) {

  if (CommandRead() == true) {
    return (NumberParser(c_buff));
  } else {
    return -1;
  }
}


int8_t CommadParser(void) {

  if (CommandRead() == true) {
    if ( c_buff[0] == 'S' && c_buff[1] == '\n') {
      return -3;
    }
    return (NumberParser(c_buff));
  } else {
    return -2;
  }
}


int8_t NumberParser(char buff[]) {
  int8_t temp;
  if ( buff[0] == '0' && buff[1] == 'x') {
    temp = (int8_t)strtol(buff, NULL, 16);
  } else {
    temp = (int8_t)strtol(buff, NULL, 10);
  }

  if (temp <= 0) {
    return -1;
  } else {
    return temp;
  }

}


bool CommandRead(void) {
  c_buff_cnt = 0;
  while(Serial. read() >= 0);
  while (c_buff_cnt < 5) {
    if (Serial.available()) {
      char inChar = (char)Serial.read();
      c_buff[c_buff_cnt] = inChar;
      c_buff_cnt++;
      if (inChar == '\n')
        return true;
    }
  }
  return false;
}


void Search(void) {
  uint8_t i, error;
  Serial.println("Scanning....");
  for (i = 0; i < 0x80; i++ )
  {
    Wire.beginTransmission(i);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Encoder.address = i;
      if (Encoder.readIDCode() == 0x39) {
        Serial.print("I2C Encoder mini found at ");
        Serial.print(Encoder.address);
        Serial.print(" ( ");
        PrintHEX(Encoder.address);
        Serial.print(" )");
        Serial.println(" !!");
      }
    }

  }
}


void PrintHEX(uint8_t i) {
  Serial.print("0x");
  if (i < 16)
    Serial.print("0");
  Serial.print(i, HEX);
}
