# I2C Encoder V2 Arduino Library
--------------------------------------------------------------------------------

## Introduction

Here you can find the library description of the [I2C Encoder V2](https://github.com/Fattoresaimon/I2CEncoderV2) for the Arduino IDE.
For more details of the functionality of the board, please read the [Datasheet](https://github.com/Fattoresaimon/I2CEncoderV2/blob/master/EncoderI2CV2_v1.6.pdf) 

The I2C Encoder V2 is available on [Tindie!](https://www.tindie.com/products/Saimon/i2cencoder-v2-connect-multiple-encoder-on-i2c-bus/)

## Initialization of the class

The library makes available the class **i2cEncoderLibV2**
To initialize the library, you have to declare an instance of the class **i2cEncoderLibV2** for each encoders.
For example:

``` C++
i2cEncoderLibV2 encoder(0x30);
```
Declaration of one encoder with the address 0x30. The jumper A4 and A5 are shorted.

``` C++
i2cEncoderLibV2 encoder(0b0110000);
```
Declaration of the same encoder but in binary format.

```C++
i2cEncoderLibV2 encoder1(0x30);
i2cEncoderLibV2 encoder2(0x32);
```
Declaration of two encoders with the address 0x30 and 0x34 in two separated variable.

```C++
i2cEncoderLibV2 encoder[2] = { i2cEncoderLibV2(0x30), i2cEncoderLibV2(0x34)};
```
Declaration of an array of the two encoders with the address 0x30 and 0x34.

## Parameters

Each class has also a public variable called **id** that is used for setting a custom id to each class.

```C++
i2cEncoderLibV2 encoder(0x30);
encoder.id=1 
```
The **id** can be useful when you declare an array of i2cEncoderLibV2 class. The **id** can be used as a index in the loops.

## Callback Configuration

This library support the callback functionality.
There is the possibility to link a function to a specific interrupt of the I2C Encoder V2, in this way the function is automatically called when the I2C Encoder V2 generates an interrupts.

A callback function must be declared as the following:

```C++
void NAME_OF_THE_FUNCTION(i2cEncoderLibV2* obj)
```

The argument **i2cEncoderLibV2* obj**  is the pointer to the class that called the method.

There are 16 possible events:

| Event   | Description   |
|:-----------:|:----------------------------------|
| onButtonRelease | Encoder push button is released |
| onButtonPush | Encoder push button is pushed |
| onButtonDoublePush | Encoder push button is double pushed |
| onIncrement | The counter value is incremented |
| onDecrement | The counter value is decremented |
| onChange | The counter value is incremented or decremented |
| onMax | The counter value reach the maximum threshold |
| onMin | The counter value reach the minimum threshold |
| onMinMax | The counter value reach the maximum or minimum threshold |
| onGP1Rise | GP1 configured as input, rising edge |
| onGP1Fall | GP1 configured as input, falling edge |
| onGP2Rise | GP2 configured as input, rising edge  |
| onGP2Fall | GP2 configured as input, falling edge  |
| onGP3Rise | GP3 configured as input, rising edge  |
| onGP3Fall | GP3 configured as input, falling edge  |
| onFadeProcess | Fade process terminated |

#### Examples:

```C++
i2cEncoderLibV2 Encoder(0x61); // Class declaration

...

// Simple callback that ist's called when the encoder is rotated
void encoder_change(i2cEncoderLibV2* obj) {
  Serial.println( obj->readCounterByte()); //Print on the terminal the counter value.
}

...

Encoder.onChange = encoder_change; //Attach the event to the callback function.

}

```

If you need to remove the link with a callback, you just need to define:
```C++
Encoder.onChange=NULL;
```

## Initialization
### void begin( uint16_t conf)
This is used for initializing the encoder by writing the configuration register of the encoder.
The parameters can be concatenated in OR mode.
The possible parameters are the following:

| Parameter | Description |
| ---------- | ------------------------------------------------------ |
| INT_DATA | The Threshold, counter step and counter value are used with integer numbers |
| FLOAT_DATA | The Threshold, counter step and counter value are used with floating numbers |
| | |
| WRAP_ENABLE | Wrap enable. When the counter value reaches the CMAX+1, restart to the CMIN and vice versa |
| WRAP_DISABLE | Wrap disable. When the counter value reaches the CMAX or CMIN, the counter stops to increasing or decreasing |
| | |
| DIRE_LEFT | Rotate left side to increase the value counter |
| DIRE_RIGHT | Rotate right side to increase the value counter |
| | |
| IPUP_DISABLE | Disable the internal pull-up on the INT pin |
| IPUP_ENABLE | Enable the internal pull-up on the INT pin |
| | |
| RMOD_X2 | Encoder in X2 mode |
| RMOD_X1 | Encoder in X1 mode |
| | |
| RGB_ENCODER | RGB illuminated encoder is soldered |
| STD_ENCODER | Standard rotary encoder is soldered |
| | |
| EEPROM_BANK1 | Select the first EEPROM bank |
| EEPROM_BANK2 | Select the second EEPROM bank |
| | |
| RESET | Reset the board |
| | |
| CLK_STRECH_ENABLE | Enable the I2C clock stretch (only v2.1) |
| CLK_STRECH_DISABLE | Disable the I2C clock stretch (only v2.1) |
| | |
| REL_MODE_ENABLE | Enable the CVAL relative mode (only v2.1)|
| REL_MODE_DISABLE | Disable the CVAL relative mode (only v2.1) |
| | |

#### Examples:

```C++
encoder.begin(i2cEncoderLibV2::INT_DATA | i2cEncoderLibV2::WRAP_DISABLE | i2cEncoderLibV2::DIRE_LEFT | i2cEncoderLibV2::IPUP_ENABLE | i2cEncoderLibV2::RMOD_X1 | i2cEncoderLibV2::STD_ENCODER | i2cEncoderLibV2::CLK_STRECH_ENABLE | i2cEncoderLibV2::REL_MODE_DISABLE  );
```

Please remember to add the class name **i2cEncoderLibV2::** before the parameter!

### void reset(void)

Reset of the board. 
In this command there is 10ms delay in order to make the board correctly restart.



## Configuration


### void writeGP1conf(uint8_t gp1)
### void writeGP2conf(uint8_t gp2)
### void writeGP3conf(uint8_t gp3)


This 3 functions are used to configure the GP pins. The parameters are the same for all of the 3 GP pins.
The interrupt configurations are used only when the pin is configured as digital input.


| Parameter   | Description   |
|:-----------:|:-------------|
|GP_PWM| Set the GP pin as PWM output|
|GP_OUT| Set the GP pin as digital output|
|GP_AN| Set the GP pin as analog input|
|GP_IN| Set the GP pin as digital input output|
|||
|GP_PULL_EN| Enable the internal pull-up of the pin|
|GP_PULL_DI| Disable the internal pull-up of the pin|
|||
|GP_INT_DI| Disable the pin interrupt|
|GP_INT_PE| Enable the interrupt at the positive edge|
|GP_INT_NE|  Enable the interrupt at the negative edge|
|GP_INT_BE|  Enable the interrupt at the positive and negative edge|

#### Examples:

```C++
encoder.writeGP1conf(i2cEncoderLibV2::GP_AN | i2cEncoderLibV2::GP_PULL_DI | i2cEncoderLibV2::GP_INT_DI);  //Configure the GP1 as analog input with the pull-up and the interrupt disable 
```

Please remember to add the class name **i2cEncoderLibV2::** before the parameter!

### void writeInterruptConfig(uint8_t interrupt)

This method  is used for enabling or disabling the interrupt source selectively. When an interrupt event occurs, the INT pin goes low and the event is stored in the status register.

| Parameter   | Description   |
|:-----------:|:-------------:|
| PUSHR | Push button of the encoder is released |
| PUSHP | Push button of the encoder is pressed |
| PUSHD | Push button of the encoder is doule pushed  |
| RINC  | Encoder is rotated in the increment direction  |
| RDEC  | Encoder is rotated in the decrement direction  |
| RMAX  | Maximum threshold is reached  |
| RMIN  | Minimum threshold is reached  |
| INT_2  | An event on the interrupt 2 register occurs |

#### Examples:

```C++
 Encoder.writeInterruptConfig(i2cEncoderLibV2::INT_2 | i2cEncoderLibV2::RMIN | i2cEncoderLibV2::RMAX | i2cEncoderLibV2::RDEC | i2cEncoderLibV2::RINC | i2cEncoderLibV2::PUSHR); 
```

Please remember to add the class name **i2cEncoderLibV2::** before the parameter!

###  void autoconfigInterrupt(void)

This method auto configures the **INTCONF** register according to the attached callback.
**For the proper use, must be called after the definition of the last event property.**

```C++
 Encoder.onIncrement = encoder_increment;
 Encoder.onDecrement = encoder_decrement;
 Encoder.onMax = encoder_max;
 Encoder.onMin = encoder_min;
 Encoder.onButtonPush = encoder_push;
 Encoder.onButtonRelease = encoder_released;
 Encoder.onButtonDoublePush = encoder_double_push;
 /* Enable the I2C Encoder V2 interrupts according to the previus attached callback */
 Encoder.autoconfigInterrupt(); 

```

### void writeAntibouncingPeriod(uint8_t bounc)

This method is used for writing the Anti-bouncing period **ANTBOUNC**.
The I2C encoder V2 will multiplies this value by 10 (value x10).

#### Examples:

```C++
encoder.writeAntibouncingPeriod(20);  //Set an anti-bouncing of 200ms 
```

### void writeDoublePushPeriod(uint8_t dperiod)

This method is used for setting the window period **DPPERIOD** of the double push of the rotary encoder switch. When the value is 0, the double push option is disabled.
The I2C encoder V2 will multiplies this value by 10 (value x10).

#### Examples:

```C++
encoder.writeDoublePushPeriod(50);  //Set a period for the double push of 500ms 
```


### void writeFadeRGB(uint8_t fade)
This method is used for setting the fade speed **FADERGB** of the RGB LED of the rotary encoder. It the value is 0, the fade option is disabled.

#### Examples:

```C++
encoder.writeFadeRGB(1);  //Fade enabled with 1ms step 
```

### void writeFadeGP(uint8_t fade)
This method is used for setting the fade speed **FADEGP** of the RGB LED of the rotary encoder. It the value is 0, the fade option is disabled.

#### Examples:

```C++
  encoder.writeFadeGP(5);  //GP Fade enabled with 5ms step 
```

## Status

### bool updateStatus(void)
Read from the encoder status register **ESTATUS** and save the value internally.
Return value is **true** in case of some event, otherwise is **false**
In case an event of the I2STATUS  register, the I2STATUS is automatically be read.

#### Examples:

```C++
  if ( Encoder.updateStatus() == true) {
  // Something happens
  }
```

### bool readStatus(Int_Status s)
Must be called after **updateStatus()**, this method is used for checking if some event occurs on the **ESTATUS** register.
Return value is **true** in case of the event occured, otherwise is **false**
Possible parameters are:

| Parameter   | Description   |
|:-----------:|:-------------:|
| PUSHR | Push button of the encoder is released |
| PUSHP | Push button of the encoder is pressed |
| PUSHD | Push button of the encoder is doule pushed  |
| RINC  | Encoder is rotated in the increment direction  |
| RDEC  | Encoder is rotated in the decrement direction  |
| RMAX  | Maximum threshold is reached  |
| RMIN  | Minimum threshold is reached  |
| INT2  | An event on the interrupt 2 register occurs |

#### Example:
```C++
 if ( Encoder.updateStatus() == true) {
      if ( Encoder.readStatus(i2cEncoderLibV2::RINC)) {
        Serial.print("Increment ");
      }
      if ( Encoder.readStatus(i2cEncoderLibV2::RDEC)) {
        Serial.print("Decrement ");
      }

      if ( Encoder.readStatus(i2cEncoderLibV2::RMAX)) {
        Serial.print("Maximum threshold: ");
      }

      if ( Encoder.readStatus(i2cEncoderLibV2::RMIN)) {
        Serial.print("Minimum threshold: ");
      }

      if ( Encoder.readStatus(i2cEncoderLibV2::PUSHR)) {
        Serial.println("Push button Released");
      }

      if ( Encoder.readStatus(i2cEncoderLibV2::PUSHP)) {
      }

      if ( Encoder.readStatus(i2cEncoderLibV2::PUSHD)) {
        Serial.println("Double push!");   
      }
```

Please remember to add the class name **i2cEncoderLibV2::** before the parameter!

### uint8_t readStatus(void)
Return the status of the register **ESTATUS**



### bool readInt2(Int2_Status s)
Must be called after **updateStatus()**, this method is used for checking if some event occurred on the secondary interrupt status **I2STATUS** register.
Return value is **true** in case of the event occurred, otherwise is **false**
Possible parameters are:

| Parameter   | Description   |
|:-----------:|:-------------:|
| GP1_POS  | Positive edge on the GP1 pin |
| GP1_NEG  | Negative edge on the GP1 pin |
| GP2_POS  | Positive edge on the GP2 pin |
| GP2_NEG  |Negative edge on the GP2 pin |
| GP3_POS  | Positive edge on the GP3 pin|
| GP3_NEG  |Negative edge on the GP3 pin |
| FADE_INT |Fade process finished   |

#### Example:
```C++
 if ( Encoder.updateStatus() == true) {
      if ( Encoder.readInt2(i2cEncoderLibV2::GP1_POS)) {
        Serial.print("GP1 positive edge");
      }
      if ( Encoder.readInt2(i2cEncoderLibV2::GP1_NEG)) {
        Serial.print("GP1 negative edge ");
      }

      if ( Encoder.readInt2(i2cEncoderLibV2::GP2_POS)) {
          Serial.print("GP2 positive edge");
      }

      if ( Encoder.readInt2(i2cEncoderLibV2::GP2_NEG)) {
        Serial.print("GP2 negative edge ");
      }

      if ( Encoder.readInt2(i2cEncoderLibV2::GP3_POS)) {
        Serial.print("GP3 positive edge");
      }

      if ( Encoder.readInt2(i2cEncoderLibV2::GP3_NEG)) {
        Serial.print("GP3 negative edge ");
      }

      if ( Encoder.readInt2(i2cEncoderLibV2::FADE_INT)) {
        Serial.println("Fade process finished");   
      }
```

Please remember to add the class name **i2cEncoderLibV2::** before the parameter!

### uint8_t readInt2(void)
Return the status of the register **I2STATUS**


### bool readFadeStatus(Fade_Status s)
When this function is called, it performs a I2C reading.
This function return **true** when the fade running, otherwise return **false**

| Parameter   | Description   |
|:-----------:|:-------------:|
| FADE_R  | Fade process status of the RGB encoder Red color |
| FADE_G  | Fade process status of the RGB encoder Green color |
| FADE_B  | Fade process status of the RGB encoder Blue color |
| FADE_GP1  | Fade process status of the GP1 |
| FADE_GP2  | Fade process status of the GP2 |
| FADE_GP3  | Fade process status of the GP3 |

Please remember to add the class name **i2cEncoderLibV2::** before the parameter!

### uint8_t readFadeStatus(void)
Return the value of the register **FSTATUS**.


## Reading methods

### int32_t readCounterLong(void)
Return the counter value in the format **int32_t**, by reading all the 4 bytes of the counter value registers.

### int16_t readCounterInt(void)
Return the counter value in the format **int16_t**, by reading the 2 LSB  of the counter value registers.
Useful when the counter register is between the values -32768 to 32767.

### int8_t readCounterByte(void)
Return the counter value in the format **int8_t**, by reading the LSB byte of the counter value register.
Useful when the counter register is between the values -128 to 127

### float readCounterFloat(void)
Return the counter value in the format **float**, by reading all the 4 bytes of the counter value registers.
For using this function you have to configure the board with the parameter **FLOAT_DATA**.

### int32_t readMax(void)
Return the maximum threshold in format **int32_t**, bye reading all the 4 bytes of the counter Max.

### float readMaxFloat(void)
Return the maximum threshold in format **float**, bye reading all the 4 bytes of the counter Max.

### int32_t readMin(void)
Return the minimum threshold in format **int32_t**, by reading all the 4 byte of the counter Min.

### float readMinFloat(void)
Return the minimum  threshold in format **float**, bye reading all the 4 bytes of the counter Min.

### int32_t readStep(void)
Return the minimum threshold in format **int32_t**, by reading all the 4 bytes of the ISTEP registers.

### float readStepFloat(void)
Return the step value in format **float**, by reading all the 4 bytes of the ISTEP registers .

### uint8_t readLEDR(void)
Return the value of the RLED register. 

### uint8_t readLEDG(void)
Return the value of the GLED register. 

### uint8_t readLEDB(void)
Return the value of the BLED register. 

### uint8_t readGP1(void)
Return the value of the GP1REG register. 
If the **GP1** is configured as input, it's possible to read the logic status of the pin: *1* when the pin is high, otherwise *0*.
If the **GP1** is configured as analog, it's possible to read the 8bit of the ADC.

### uint8_t readGP2(void)
Return the value of the GP2REG register. 
If the **GP2** is configured as input, it's possible to read the logic status of the pin: *1* when the pin is high, otherwise *0*.
If the **GP2** is configured as analog, it's possible to read the 8bit of the ADC.

### uint8_t readGP3(void)
Return the value of the GP3REG register. 
If the **GP3** is configured as input, it's possible to read the logic status of the pin: *1* when the pin is high, otherwise *0*.
If the **GP3** is configured as analog, it's possible to read the 8bit of the ADC. 

### uint8_t readAntibouncingPeriod(void)
Return the value of the ANTBOUNC register. 

### uint8_t readDoublePushPeriod(void)
Return the value of the DPPERIOD register. 

### uint8_t readFadeRGB(void)
Return the value of the FADERGB register. 

### uint8_t readFadeGP(void)
Return the value of the FADEGP register. 

### uint8_t readIDCode(uint8_t add)
Return the ID code of the I2C Encoder V2.1, that is 0x53
Avaiable only on the V2.1

### uint8_t readVersion(uint8_t add)
Return the version of the board. 
Avaiable only on the V2.1

### uint8_t readEEPROM(uint8_t add)
Return the value of the EEPROM register. 
This function automatically manage the setting of the first and second memory bank.




## Writing methods

###  void writeCounter(int32_t counter)
Write the counter value register with a  **int32_t** number. All of the 4 bytes are wrote.

###  void writeCounter(float counter)
Write the counter value register with a  **float** number. All of the 4 bytes are wrote.

###  void writeMax(int32_t max)
Write the Max register with a  **int32_t** number. All of the 4 bytes are wrote.

###  void writeMax(float max)
Write the Max register with a  **float** number. All of the 4 bytes are wrote.

###  void writeMin(int32_t min)
Write the Min register with a  **int32_t** number. All of the 4 bytes are wrote.

###  void writeMin(float min)
Write the Min register with a  **float** number. All of the 4 bytes are wrote.

###  void writeStep(int32_t step)
Write the increment step  with a  **int32_t** number. All of the 4 bytes are wrote.

###  void writeStep(float step)
Write the increment step with a  **float** number. All of the 4 bytes are wrote.

### void writeLEDR(uint8_t rled)
Write the PWM value of the RLED of the RGB encoder. When 0 means PWM at 0%, LED off while 0xFF means PWM at 100% and LED ON.

### void writeLEDG(uint8_t gled)
Write the PWM value of the GLED of the RGB encoder. When 0 means PWM at 0%, LED off while 0xFF means PWM at 100% and LED ON.

### void writeLEDB(uint8_t bled)
Write the PWM value of the BLED of the RGB encoder. When 0 means PWM at 0%, LED off while 0xFF means PWM at 100% and LED ON.

### void writeRGBCode(uint32_t rgb)
Write a 24bit RGB color in the format 0xRRGGBB.

### void writeGP1(uint8_t gp1)
Write the GP1REG register.
If the GP1 is configure as PWM with this method it's possible to write the PWM value.
If the GP1 is configure as output with this method it's possible to write the logic status: 1 for high, while 0 for low.

### void writeGP2(uint8_t gp2)
Write the GP2REG register.
If the GP2 is configure as PWM with this method it's possible to write the PWM value.
If the GP2 is configure as output with this method it's possible to write the logic status: 1 for high, while 0 for low.

### void writeGP3(uint8_t gp3)
Write the GP3REG register.
If the GP3 is configure as PWM with this method it's possible to write the PWM value.
If the GP3 is configure as output with this method it's possible to write the logic status: 1 for high, while 0 for low.

### void writeAntibouncingPeriod(uint8_t bounc)
Write the ANTBOUNC register.

### void writeDoublePushPeriod(uint8_t dperiod)
Write the DPPERIOD register.

### void writeFadeRGB(uint8_t fade)
Write the FADERGB register.

### void writeFadeGP(uint8_t fade)
Write the FADEGP register.

### void writeGammaRLED(GAMMA_PARAMETER Gamma)
### void writeGammaGLED(GAMMA_PARAMETER Gamma)
### void writeGammaBLED(GAMMA_PARAMETER Gamma)
### void writeGammaGP1(GAMMA_PARAMETER Gamma)
### void writeGammaGP2(GAMMA_PARAMETER Gamma)
### void writeGammaGP3(GAMMA_PARAMETER Gamma)

This method is used to set a gamma correction for the RGB led of the encoder and for the GP pins.

| Parameter   | Description   |
|:-----------:|:-------------|
| GAMMA_OFF   | Gamma correction is OFF |
| GAMMA_1   | Gamma is 1, in this case the PWM is linear |
| GAMMA_1_8   | Gamma is 1.8 |
| GAMMA_2   | Gamma is 2 |
| GAMMA_2_2   | Gamma is 2.2 |
| GAMMA_2_4   | Gamma is 2.4 |
| GAMMA_2_6 | Gamma is 2.6 |
| GAMMA_2_8   | Gamma is 2.8 |

#### Examples:

```C++
  Encoder.writeGammaRLED(i2cNavKey::GAMMA_1_8);
  Encoder.writeGammaGLED(i2cNavKey::GAMMA_1_8);
  Encoder.writeGammaBLED(i2cNavKey::GAMMA_1_8);
```

### void writeEEPROM(uint8_t add, uint8_t data)
Write the EEPROM memory.
The input parameter *add* is the address. This method automatically change the first or the second bank.
The input parameter *data* is the data that will be written.
If the I2C clock stretch is disabled, a delay of 5ms is added.

