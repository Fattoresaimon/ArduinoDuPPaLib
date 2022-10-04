# I2C Encoder V2 Arduino Library
--------------------------------------------------------------------------------

## Introduction

Here you can find the library description of the [I2C Encoder V2](https://github.com/Fattoresaimon/I2CEncoderV2) for the Arduino IDE.
For more details of the functionality of the board, please read the [Datasheet](https://github.com/Fattoresaimon/I2CEncoderV2/blob/master/EncoderI2CV2_v1.6.pdf) 

The I2C Encoder V2 is available on [Tindie!](https://www.tindie.com/products/Saimon/i2cencoder-v2-connect-multiple-encoder-on-i2c-bus/)

## Initialization of the class

The library makes available the class **i2cEncoderLibV2**
To initialize the library, you must declare an instance of the class **i2cEncoderLibV2** for each encoder.
For example:

Instantiate one encoder with the I2C address 0x30 (Jumpers A4 and A5 are shorted):
``` C++
i2cEncoderLibV2 encoder(0x30);
```

Instantiate of the same encoder but in binary format:
``` C++
i2cEncoderLibV2 encoder(0b0110000);
```

Instantiate two encoders with I2C addresses 0x30 and 0x34 in two separate variables:
```C++
i2cEncoderLibV2 encoder1(0x30);
i2cEncoderLibV2 encoder2(0x34);
```

Declaration of an array of two encoders with the I2C addresses 0x30 and 0x34.
```C++
i2cEncoderLibV2 encoder[2] = { i2cEncoderLibV2(0x30), i2cEncoderLibV2(0x34)};
```

## Parameters

Each class defines a public variable **id** that is used for setting a custom id to each class.

```C++
i2cEncoderLibV2 encoder(0x30);
encoder.id=1;
```
It can be useful when declaring an array of i2cEncoderLibV2 objects; such as being used **id** as an index in loops.

## Callback Configuration

This library supports callback functionality.
It is possible to link a function to a specific interrupt of the I2C Encoder V2, in this way the appropriate function is automatically called when the I2C Encoder V2 generates an interrupt.

A callback function must be declared as follows:

```C++
void NAME_OF_THE_FUNCTION(i2cEncoderLibV2* obj);
```

The **i2cEncoderLibV2* obj** argument is the pointer to the class that called the method.

There are 16 possible events:

|       Event        | Description                                                     |
|:------------------:|:----------------------------------------------------------------|
|  onButtonRelease   | Encoder push button was released                                |
|    onButtonPush    | Encoder push button was pushed                                  |
| onButtonDoublePush | Encoder push button was double pushed                           |
|    onIncrement     | The counter value was incremented                               |
|    onDecrement     | The counter value was decremented                               |
|      onChange      | The counter value was either incremented or decremented         |
|       onMax        | The counter value is at the maximum threshold                   |
|       onMin        | The counter value is at the minimum threshold                   |
|      onMinMax      | The counter value is at either the maximum or minimum threshold |
|     onGP1Rise      | If GP1 is configured as input - triggered by rising edge        |
|     onGP1Fall      | If GP1 is configured as input - triggered by falling edge       |
|     onGP2Rise      | If GP2 is configured as input - triggered by rising edge        |
|     onGP2Fall      | If GP2 is configured as input - triggered by falling edge       |
|     onGP3Rise      | If GP3 is configured as input - triggered by rising edge        |
|     onGP3Fall      | If GP3 is configured as input - triggered by falling edge       |
|   onFadeProcess    | Fade process terminated                                         |

#### Examples:

```C++
i2cEncoderLibV2 Encoder(0x61); // Class declaration

// Simple callback that is called whenever the encoder is rotated
void encoder_change(i2cEncoderLibV2* obj) {
  Serial.println( obj->readCounterByte()); //Print the counter value to the serial output
}

Encoder.onChange = encoder_change; //Attach the event to the callback function.
```

If you need to remove the link to a callback, you just need to define:
```C++
Encoder.onChange=NULL;
```

## Initialization
### void begin( uint16_t conf)
This is used for initializing the encoder by writing to the configuration register of the encoder.
The parameters can be combined with OR.
The possible parameters are:

| Parameter          | Description                                                                                            |
|--------------------|--------------------------------------------------------------------------------------------------------|
| INT_DATA           | The threshold, counter step and counter values are stored as integers                                  |
| FLOAT_DATA         | The threshold, counter step and counter values are stored as floating point                            |
| WRAP_ENABLE        | Enables wrap. When the counter value reaches CMAX+1, set counter to CMIN and vice versa                |
| WRAP_DISABLE       | Disables wrap. When the counter value reaches CMAX or CMIN, the counter stops increasing or decreasing |
| DIRE_LEFT          | Rotating the encoder to the left (CCW) increases the value of the counter                              |
| DIRE_RIGHT         | Rotating the encoder to the right (CW) increases the value of the counter                              |
| IPUP_DISABLE       | Disables the INT pin's internal pull-up                                                                |
| IPUP_ENABLE        | Enables the INT pin's internal pull-up                                                                 |
| RMOD_X2            | Encoder in X2 mode                                                                                     |
| RMOD_X1            | Encoder in X1 mode                                                                                     |
| RGB_ENCODER        | RGB rotary encoder is attached                                                                         |
| STD_ENCODER        | Standard rotary encoder is attached                                                                    |
| EEPROM_BANK1       | Selects the first EEPROM bank                                                                          |
| EEPROM_BANK2       | Selects the second EEPROM bank                                                                         |
| RESET              | Reset the board                                                                                        |
| CLK_STRECH_ENABLE  | Enables I2C clock stretching (only on v2.1+)                                                           |
| CLK_STRECH_DISABLE | Disables I2C clock stretching (only on v2.1+)                                                          |
| REL_MODE_ENABLE    | Enables the CVAL relative mode (only on v2.1+)                                                         |
| REL_MODE_DISABLE   | Disables the CVAL relative mode (only on v2.1+)                                                        |

#### Examples:

```C++
encoder.begin(
  i2cEncoderLibV2::INT_DATA         // The threshold, counter step and counter values are stored as integers  
  | i2cEncoderLibV2::WRAP_ENABLE    // Enables wrap. When the counter value reaches CMAX+1, set counter to CMIN and vice versa
  | i2cEncoderLibV2::DIRE_RIGHT     // Rotating the encoder to the right (CW) increases the value of the counter
  | i2cEncoderLibV2::IPUP_ENABLE    // Enables the INT pin's internal pull-up
  | i2cEncoderLibV2::RMOD_X1        // Encoder in X1 mode
  | i2cEncoderLibV2::RGB_ENCODER);  // RGB rotary encoder is attached
```

Please remember to add the class name **i2cEncoderLibV2::** before the parameter!

### void reset(void)

Reset the board. 
Note: There is a 10ms delay to ensure the board restarts correctly.

## Configuration


### void writeGP1conf(uint8_t gp1)
### void writeGP2conf(uint8_t gp2)
### void writeGP3conf(uint8_t gp3)


This three functions are used to configure the behaviour of the GP pins.
Interrupt configuration is only required if the pin is configured as a digital input.


| Parameter  | Description                                                         |
|:----------:|:--------------------------------------------------------------------|
|   GP_PWM   | Configure the GP pin as a PWM output                                |
|   GP_OUT   | Configure the GP pin as a digital output                            |
|   GP_AN    | Configure the GP pin as a analog input                              |
|   GP_IN    | Configure the GP pin as a digital input                             |
| GP_PULL_EN | Enable the internal pull-up on the GP pin                           |
| GP_PULL_DI | Disable the internal pull-up on the GP pin                          |
| GP_INT_DI  | Disable interrupt triggering on the GP pin                          |
| GP_INT_PE  | Enable interrupt triggering on the positive edge                    |
| GP_INT_NE  | Enable interrupt triggering on the negative edge                    |
| GP_INT_BE  | Enable interrupt triggering on both the positive and negative edges |

#### Examples:

```C++
encoder.writeGP1conf(i2cEncoderLibV2::GP_AN | i2cEncoderLibV2::GP_PULL_DI | i2cEncoderLibV2::GP_INT_DI);  //Configure GP1 as an analog input with the pull-up and interrupts disabled
```

Please remember to add the class name **i2cEncoderLibV2::** before the parameter!

### void writeInterruptConfig(uint8_t interrupt)

This method is used for selectively enabling or disabling interrupt sources. When an interrupt event occurs, the INT pin is pulled low and the event is stored in the status register.

| Parameter |                    Description                    |
|:---------:|:-------------------------------------------------:|
|   PUSHR   |            Encoder button is released             |
|   PUSHP   |             Encoder button is pressed             |
|   PUSHD   |          Encoder button is double pushed          |
|   RINC    |   Encoder is rotated in the increment direction   |
|   RDEC    |   Encoder is rotated in the decrement direction   |
|   RMAX    |           Maximum threshold is reached            |
|   RMIN    |           Minimum threshold is reached            |
|   INT_2   | An event on the interrupt 2 register has occurred |

#### Examples:

```C++
Encoder.writeInterruptConfig(i2cEncoderLibV2::INT_2 | i2cEncoderLibV2::RMIN | i2cEncoderLibV2::RMAX | i2cEncoderLibV2::RDEC | i2cEncoderLibV2::RINC | i2cEncoderLibV2::PUSHR); 
```

Please remember to add the class name **i2cEncoderLibV2::** before the parameter!

###  void autoconfigInterrupt(void)

This method autoconfigures the **INTCONF** register according to the attached callback.
**For proper use, it must be called after the definition of the last event property.**

```C++
Encoder.onIncrement = encoder_increment;
Encoder.onDecrement = encoder_decrement;
Encoder.onMax = encoder_max;
Encoder.onMin = encoder_min;
Encoder.onButtonPush = encoder_push;
Encoder.onButtonRelease = encoder_released;
Encoder.onButtonDoublePush = encoder_double_push;

Encoder.autoconfigInterrupt(); // Enables the I2C Encoder V2 interrupts according to the previously attached callback
```

### void writeAntibouncingPeriod(uint8_t bounc)

This method sets the Anti-bouncing period **ANTBOUNC** to the provided value in milliseconds.

NOTE:If using I2C encoder V2+, the provided value will be multiplied by 10.

#### Examples:

```C++
encoder.writeAntibouncingPeriod(20);  //Set an anti-bouncing period of 200ms
```

### void writeDoublePushPeriod(uint8_t dperiod)

This method sets the time period **DPPERIOD** in milliseconds; if two button pushes occur within the specified period, it is recognized as a double push.
If the value is set to 0, double push detection is disabled.

NOTE:If using I2C encoder V2+, the provided value will be multiplied by 10.

#### Examples:

```C++
encoder.writeDoublePushPeriod(50);  //Set the double push detection window to 500ms 
```


### void writeFadeRGB(uint8_t fade)
This method sets the fade speed **FADERGB** of the rotary encoder's RGB LED. 
If the value is set to 0, the fade option is disabled.

#### Examples:

```C++
encoder.writeFadeRGB(1);  //Fade enabled with 1ms step 
```

### void writeFadeGP(uint8_t fade)
This method is used for setting the fade speed **FADEGP** of the rotary encoder's RGB LED. If the value is set to 0, the fade option is disabled.

#### Examples:

```C++
encoder.writeFadeGP(5);  //GP Fade enabled with 5ms step 
```

## Status

### bool updateStatus(void)
Reads from the encoder's status register **ESTATUS** and saves the value internally.
Returns **true** if an event occurred, otherwise **false**.
In the case of an event from the I2STATUS register, the I2STATUS will automatically be read.

#### Examples:

```C++
if (Encoder.updateStatus() == true) {
// Do something
}
```

### bool readStatus(Int_Status s)
Must be called after **updateStatus()** 
This method is used for checking if an event occurs on the **ESTATUS** register.
Returns **true** in the case an event occurred, otherwise **false**
Possible parameters are:

| Parameter |                  Description                   |
|:---------:|:----------------------------------------------:|
|   PUSHR   |    Push button of the encoder was released     |
|   PUSHP   |     Push button of the encoder was pressed     |
|   PUSHD   |  Push button of the encoder was double pushed  |
|   RINC    | Encoder was rotated in the increment direction |
|   RDEC    | Encoder was rotated in the decrement direction |
|   RMAX    |       Maximum threshold has been reached       |
|   RMIN    |       Minimum threshold has been reached       |
|   INT2    |  An event on the interrupt 2 register occurs   |

#### Example:
```C++
if (Encoder.updateStatus() == true) {

  if (Encoder.readStatus(i2cEncoderLibV2::RINC)) {
    Serial.print("Counter incremented to ");
    Serial.println(Encoder.readCounterByte());
  }

  if (Encoder.readStatus(i2cEncoderLibV2::RDEC)) {
    Serial.print("Counter decremented to");
    Serial.println(Encoder.readCounterByte());
  }

  if (Encoder.readStatus(i2cEncoderLibV2::RMAX)) {
    Serial.println("Maximum threshold reached");
  }

  if (Encoder.readStatus(i2cEncoderLibV2::RMIN)) {
    Serial.println("Minimum threshold reached");
  }

  if (Encoder.readStatus(i2cEncoderLibV2::PUSHP)) {
    Serial.println("Push button pushed");
  }

  if (Encoder.readStatus(i2cEncoderLibV2::PUSHR)) {
    Serial.println("Push button released");
  }

  if (Encoder.readStatus(i2cEncoderLibV2::PUSHD)) {
    Serial.println("Push button double pushed");
  }
```

Please remember to add the class name **i2cEncoderLibV2::** before each parameter!

### uint8_t readStatus(void)
Returns the value of the **ESTATUS** register.



### bool readInt2(Int2_Status s)
Must be called after **updateStatus()**, this method is used for checking if an event occurred on the secondary interrupt status **I2STATUS** register.
Returns **true** if an event occurred, otherwise returns **false**
Possible values for **readInt2** are:

|  Value   |      Description      |
|:--------:|:---------------------:|
| GP1_POS  | Positive edge on GP1  |
| GP1_NEG  | Negative edge on GP1  |
| GP2_POS  | Positive edge on GP2  |
| GP2_NEG  | Negative edge on GP2  |
| GP3_POS  | Positive edge on GP3  |
| GP3_NEG  | Negative edge on GP3  |
| FADE_INT | Fade process finished |

#### Example:
```C++
if (Encoder.updateStatus() == true) {
  if (Encoder.readInt2(i2cEncoderLibV2::GP1_POS)) {
    Serial.print("GP1 triggered on positive/rising edge");
  }

  if (Encoder.readInt2(i2cEncoderLibV2::GP1_NEG)) {
    Serial.print("GP1 triggered on negative/falling edge");
  }

  if (Encoder.readInt2(i2cEncoderLibV2::GP2_POS)) {
      Serial.print("GP2 triggered on positive/rising edge");
  }

  if (Encoder.readInt2(i2cEncoderLibV2::GP2_NEG)) {
    Serial.print("GP2 triggered on negative/falling edge");
  }

  if (Encoder.readInt2(i2cEncoderLibV2::GP3_POS)) {
    Serial.print("GP3 triggered on positive/rising edge");
  }

  if (Encoder.readInt2(i2cEncoderLibV2::GP3_NEG)) {
    Serial.print("GP3 triggered on negative/falling edge");
  }

  if (Encoder.readInt2(i2cEncoderLibV2::FADE_INT)) {
    Serial.println("Fade process finished");   
  }
}
```

Remember to add the class name **i2cEncoderLibV2::** before the parameter!

### uint8_t readInt2(void)
Returns the status of the **I2STATUS** register 


### bool readFadeStatus(Fade_Status s)
This function returns **true** if the fade process is actively running, otherwise returns **false**.

| Parameter |                Description                |
|:---------:|:-----------------------------------------:|
|  FADE_R   |  RGB encoder red LED fade process status  |
|  FADE_G   | RGB encoder green LED fade process status |
|  FADE_B   | RGB encoder blue LED fade process status  |
| FADE_GP1  |          GP1 fade process status          |
| FADE_GP2  |          GP2 fade process status          |
| FADE_GP3  |          GP3 fade process status          |

Please remember to add the class name **i2cEncoderLibV2::** before each parameter!

### uint8_t readFadeStatus(void)
Returns the value of the **FSTATUS** register.


## Reading methods

### int32_t readCounterLong(void)
Returns the counter value as an **int32_t**, by reading all the 4 bytes of the counter value registers.

### int16_t readCounterInt(void)
Returns the counter value as an **int16_t**, by reading the two LSB of the counter value registers.
Useful when the counter register is between the values -32768 to 32767.

### int8_t readCounterByte(void)
Returns the counter value as an **int8_t**, by reading the LSB byte of the counter value registers.
Useful when the counter register is between the values -128 to 127

### float readCounterFloat(void)
Returns the counter value as a **float**, by reading all 4 bytes of the counter value register.
To use this function, you must first configure the board with the **FLOAT_DATA** parameter.

### int32_t readMax(void)
Returns the maximum threshold as an **int32_t**, by reading all 4 bytes of the Max registers.

### float readMaxFloat(void)
Returns the maximum threshold as a **float**, by reading all 4 bytes of the Max registers.

### int32_t readMin(void)
Returns the minimum threshold as an **int32_t**, by reading all 4 byte of the Min registers.

### float readMinFloat(void)
Returns the minimum threshold as a **float**, by reading all 4 bytes of the Min registers.

### int32_t readStep(void)
Returns the minimum threshold as an **int32_t**, by reading all 4 bytes of the **ISTEP** registers.

### float readStepFloat(void)
Returns the step value as a **float**, by reading all 4 bytes of the **ISTEP** registers.

### uint8_t readLEDR(void)
Returns the value of the **RLED** register. 

### uint8_t readLEDG(void)
Returns the value of the **GLED** register. 

### uint8_t readLEDB(void)
Returns the value of the **BLED** register. 

### uint8_t readGP1(void)
Returns the value of the **GP1REG** register. 
If **GP1** is configured as a digital input, it returns the logic value of the pin: *1* when the pin is high, otherwise *0*.
If **GP1** is configured as an analog input, it returns the ADC as an 8bit value.

### uint8_t readGP2(void)
Returns the value of the **GP2REG** register. 
If **GP2** is configured as a digital input, it returns the logic value of the pin: *1* when the pin is high, otherwise *0*.
If **GP2** is configured as an analog input, it returns the ADC as an 8bit value.

### uint8_t readGP3(void)
Returns the value of the **GP3REG** register. 
If **GP3** is configured as a digital input, it returns the logic value of the pin: *1* when the pin is high, otherwise *0*.
If **GP3** is configured as an analog input, it returns the ADC as an 8bit value.

### uint8_t readAntibouncingPeriod(void)
Returns the value of the **ANTBOUNC** register. 

### uint8_t readDoublePushPeriod(void)
Returns the value of the **DPPERIOD** register. 

### uint8_t readFadeRGB(void)
Returns the value of the **FADERGB** register. 

### uint8_t readFadeGP(void)
Returns the value of the **FADEGP** register. 

### uint8_t readIDCode(uint8_t add)
Returns the ID code of the I2CEncoder (0x53) - Available only on V2.1+

### uint8_t readVersion(uint8_t add)
Returns the version of the board - Available only on V2.1+

### uint8_t readEEPROM(uint8_t add)
Returns the value of the EEPROM register. 
This function automatically manages the first and second memory banks.


## Writing methods

###  void writeCounter(int32_t counter)
Write an **int32_t** to the counter value register. All 4 bytes are written.

###  void writeCounter(float counter)
Write a **float** to the counter value register. All 4 bytes are written.

###  void writeMax(int32_t max)
Write an **int32_t** to the Max register. All 4 bytes are written.

###  void writeMax(float max)
Write a **float** to the Max register. All 4 bytes are written.

###  void writeMin(int32_t min)
Write an **int32_t** to the Min register. All 4 bytes are written.

###  void writeMin(float min)
Write a **float** to the Min register. All 4 bytes are written.

###  void writeStep(int32_t step)
Write an **int32_t** to the increment step register. All 4 bytes are written.

###  void writeStep(float step)
Write a **float** to the increment step register. All 4 bytes are written.

### void writeLEDR(uint8_t rled)
Write the PWM value for the RGB encoder's red LED. 0 equals 0% PWM/Always off, while 0xFF equals 100% PWM/Always on.

### void writeLEDG(uint8_t gled)
Write the PWM value for the RGB encoder's green LED. 0 equals 0% PWM/Always off, while 0xFF equals 100% PWM/Always on.

### void writeLEDB(uint8_t bled)
Write the PWM value for the RGB encoder's blue LED. 0 equals 0% PWM/Always off, while 0xFF equals 100% PWM/Always on.

### void writeRGBCode(uint32_t rgb)
Write a 24bit RGB color in the format 0xRRGGBB.

### void writeGP1(uint8_t gp1)
Write to the **GP1REG** register.

If **GP1** is configured as PWM, this method writes the PWM value.

If **GP1** is configured as a digital output, passing **1** will set the pin high, while passing **0** sets the pin low.

### void writeGP2(uint8_t gp2)
Write to the **GP2REG** register.

If **GP2** is configured as PWM, this method writes the PWM value.

If **GP2** is configured as a digital output, passing **1** will set the pin high, while passing **0** sets the pin low.

### void writeGP3(uint8_t gp3)
Write to the **GP3REG** register.

If **GP3** is configured as PWM, this method writes the PWM value.

If **GP3** is configured as a digital output, passing **1** will set the pin high, while passing **0** sets the pin low.

### void writeAntibouncingPeriod(uint8_t bounc)
Write to the **ANTBOUNC** register.

### void writeDoublePushPeriod(uint8_t dperiod)
Write to the **DPPERIOD** register.

### void writeFadeRGB(uint8_t fade)
Write to the **FADERGB** register.

### void writeFadeGP(uint8_t fade)
Write to the **FADEGP** register.

### void writeGammaRLED(GAMMA_PARAMETER Gamma)
### void writeGammaGLED(GAMMA_PARAMETER Gamma)
### void writeGammaBLED(GAMMA_PARAMETER Gamma)
### void writeGammaGP1(GAMMA_PARAMETER Gamma)
### void writeGammaGP2(GAMMA_PARAMETER Gamma)
### void writeGammaGP3(GAMMA_PARAMETER Gamma)

These methods are used to set the gamma correction for the RGB encoder's LEDs and for the GP pins.

| Parameter | Description             |
|:---------:|:------------------------|
| GAMMA_OFF | Gamma correction is OFF |
|  GAMMA_1  | Gamma is 1 (linear)     |
| GAMMA_1_8 | Gamma is 1.8            |
|  GAMMA_2  | Gamma is 2              |
| GAMMA_2_2 | Gamma is 2.2            |
| GAMMA_2_4 | Gamma is 2.4            |
| GAMMA_2_6 | Gamma is 2.6            |
| GAMMA_2_8 | Gamma is 2.8            |

#### Examples:

```C++
Encoder.writeGammaRLED(i2cEncoderLibV2::GAMMA_1_8);
Encoder.writeGammaGLED(i2cEncoderLibV2::GAMMA_1_8);
Encoder.writeGammaBLED(i2cEncoderLibV2::GAMMA_1_8);
```

### void writeEEPROM(uint8_t add, uint8_t data)
Write to the EEPROM memory.
The *add* parameter is the address. This method will automatically change to the first or the second bank.
The *data* parameter is the data that will be written.
If I2C clock stretching is disabled, a delay of 5ms is added.

