# I2C NavKey Arduino Library

--------------------------------------------------------------------------------

## Introduction

Here you can find the library description of the [I2C NavKey](https://github.com/Fattoresaimon/I2CNavKey) for the Arduino IDE.
For more details of the functionality of the board, please read the [Datasheet](https://github.com/Fattoresaimon/I2CNavKey/blob/master/I2CNavKey_v1.0.pdf) 

The I2C NavKey is available on [Tindie!](https://www.tindie.com/products/16624/)

## Initialization of the class

The library makes available the class **i2cNavKey**
To initialize the library, you have to declare an instance of the class **i2cNavKey** for each encoders.
For example:

``` C++
i2cNavKey navkey(0x10);
```
Declaration of one encoder with the address 0x10. All the jumpers are open.

``` C++
i2cNavKey navkey(0b0010000);
```
Declaration of the same I2CNavKey but in binary format.

```C++
i2cNavKey navkey1(0x11); //A0 shorted
i2cNavKey navkey2(0x12); //A1 shorted
```
Declaration of two I2CNavKey with the address 0x11 and 0x12 in two separated variable.

```C++
i2cNavKey encoder[2] = { i2cNavKey(0x11), i2cNavKey(0x12)};
```
Declaration of an array of the two I2CNavKey with the address 0x11 and 0x12.

## Callback Configuration

This library support the callback functionality.
There is the possibility to link a function to a specific interrupt of the I2CNavKey, in this way the function is automatically called when the I2CNavKey  generates an interrupts.

A callback function must be declared as the following:

```C++
void NAME_OF_THE_FUNCTION(i2cNavKey* obj)
```

The argument **i2cNavKey* obj**  is the pointer to the class that called the method.

There are 26 possible events:

| Event   | Description   |
|:-----------:|:----------------------------------|
| onArrowsPush | One of the 4 direction keys is pressed |
| onArrowsRelese	| One of the 4 direction keys is released |
| onUpPush | Arrow up is pressed |
| onUpRelease | Arrow up is released |
| onDownPush | Arrow down is pressed |
| onDownRelease | Arrow down is released |
| onRightPush | Arrow right is pressed |
| onRightRelease | Arrow right is released |
| onLeftPush | Arrow left is pressed |
| onLeftRelease | Arrow left is released |
| onCentralPush | central button is pressed |
| onCentralRelease | central button is released |
| onCentralDoublePush | central button is double pushed |
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
i2cNavKey navkey(0x10);// Class declaration

...

// Simple callback that ist's called when the encoder is rotated
void navkey_encoder_change(i2cNavKey* obj) {
  Serial.println( obj->readCounterByte()); //Print on the terminal the counter value.
}

...

navkey.onChange = navkey_encoder_change; //Attach the event to the callback function.


}

```

If you need to remove the link with a callback, you just need to define:
```C++
navkey.onChange=NULL;
```



## Initialization

### void begin( uint8_t conf)
This is used for initializing the I2CNavKey by writing the configuration register.
The parameters can be concatenated in OR mode.
The possible parameters are the following:

| Parameter | Description |
|:----------:|:------------------------------------------------------ |
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
| CLK_STRECH_ENABLE | Enable the I2C clock stretch  |
| CLK_STRECH_DISABLE | Disable the I2C clock stretch  |
| | |
| EEPROM_BANK1 | Select the first EEPROM bank |
| EEPROM_BANK2 | Select the second EEPROM bank |
| | |
| RESET | Reset the board |

#### Examples:

```C++
navkey.begin(i2cNavKey::INT_DATA | i2cNavKey::WRAP_ENABLE | i2cNavKey::DIRE_RIGHT | i2cNavKey::IPUP_ENABLE);
```

### void reset(void)
Reset of the board. 
In this command there is 8ms delay in order to make the board correctly restart.

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
  navkey.writeGP1conf(i2cNavKey::GP_IN | i2cNavKey::GP_PULL_EN | i2cNavKey::GP_INT_BE );  //Configure the GP1 as digital input with the pull-up enable and the interrupt enabled on both edges 
 
 navkey.writeGP2conf(i2cNavKey::GP_PWM); //Configure the GP2 as PWM output
  
```

### void writeInterruptConfig(uint8_t interrupt)

This method is used for enabling or disabling the interrupt source selectively. When an interrupt event occurs, the INT pin goes low and the event is stored in the status register.

| Parameter   | Description   |
|:-----------:|:-------------|
| UPR   | Arrow up is released |
| UPP   | Arrow up is pressed |
| DNR   | Arrow down is released |
| DNP   | Arrow down is pressed |
| RTR   | Arrow right is released |
| RTP   | Arrow right is pressed |
| LTR   | Arrow left is released |
| LTP   | Arrow left is pressed |
| CTRR  | Central button is released |
| CTRP  | Central button is pressed |
| CTRDP | Central button is double pushed |
| RINC  | Encoder is rotated in the increment direction  |
| RDEC  | Encoder is rotated in the decrement direction  |
| RMAX  | Maximum threshold is reached  |
| RMIN  | Minimum threshold is reached  |
| INT_2  | An event on the interrupt 2 register occurs |

###  void autoconfigInterrupt(void)
This method auto configures the **INTCONF** register according to the attached callback.
**For the proper use, must be called after the definition of the last event property.**

```C++
 navkey.onUpPush = navkey_Up;
 navkey.onDownPush = navkey_Down;
 navkey.onLeftPush = navkey_Left;
 navkey.onRightPush = navkey_Right;
 navkey.onIncrement = navkey_increment;
 navkey.onDecrement = navkey_decrement;
 navkey.onMax = navkey_max;
 navkey.onMin = navkey_min;
 navkey.onButtonPush = navkey_push;
 navkey.onButtonDoublePush = navkey_double_push;
 /* Enable the I2C Encoder V2 interrupts according to the previus attached callback */
 navkey.autoconfigInterrupt(); 

```

### void writeDoublePushPeriod(uint8_t dperiod)

This method is used for setting the window period **DPPERIOD** of the double push of the central button. When the value is 0, the double push option is disabled.
The I2C NavKey will multiplies this value by 10 (value x10).

#### Examples:

```C++
navkey.writeDoublePushPeriod(50);  //Set a period for the double push of 500ms 
```


### void writeFadeGP(uint8_t fade)

This method is used for setting the fade speed **FADEGP** of the RGB LED of the rotary encoder. It the value is 0, the fade option is disabled.
#### Examples:

```C++
  navkey.writeFadeGP(5);  //GP Fade enabled with 5ms step 
```


### void writeGammaGP1(uint8_t fade)
### void writeGammaGP2(uint8_t fade)
### void writeGammaGP3(uint8_t fade)

This method is used to set a gamma correction in case the GP pin is set to PWM

| Parameter   | Description   |
|:-----------:|:-------------|
| GAMMA_1   | Gamma is 1, in thi case the PWM is lenear |
| GAMMA_1_8   | Gamma is 1.8 |
| GAMMA_2   | Gamma is 2 |
| GAMMA_2_2   | Gamma is 2.2 |
| GAMMA_2_4   | Gamma is 2.4 |
| GAMMA_2_6 | Gamma is 2.6 |
| GAMMA_2_8   | Gamma is 2.8 |

#### Examples:

```C++
  navkey.writeGammaGP1(i2cNavKey::GAMMA_1);
  navkey.writeGammaGP2(i2cNavKey::GAMMA_2);
  navkey.writeGammaGP3(i2cNavKey::GAMMA_2_2);
```


## Status

### bool updateStatus(void)
Read the I2CNavKey status register **ISTATUS**. In case of events the attached callback is called.
Return value is **true** in case of some event, otherwise is **false**
In case an event of the I2STATUS  register, the I2STATUS is automatically be read.

#### Examples:

```C++
  if ( navkey.updateStatus() == true) {
  // Something happens
  }
```



### bool readStatus(uint8_t s)

Must be called after **updateStatus()**, this method is used for checking if some event occurs on the **ISTATUS** register.
Return value is **true** in case of the event occurred, otherwise is **false**
Possible parameters are:

| Parameter   | Description   |
|:-----------:|:-------------|
| UPR   | Arrow up is released |
| UPP   | Arrow up is pressed |
| DNR   | Arrow down is released |
| DNP   | Arrow down is pressed |
| RTR   | Arrow right is released |
| RTP   | Arrow right is pressed |
| LTR   | Arrow left is released |
| LTP   | Arrow left is pressed |
| CTRR  | Central button is released |
| CTRP  | Central button is pressed |
| CTRDP | Central button is double pushed |
| RINC  | Encoder is rotated in the increment direction  |
| RDEC  | Encoder is rotated in the decrement direction  |
| RMAX  | Maximum threshold is reached  |
| RMIN  | Minimum threshold is reached  |
| INT_2  | An event on the interrupt 2 register occurs |

#### Example:
```C++
 if ( navkey.updateStatus() == true) {
      if ( navkey.readStatus(RINC)) {
        Serial.print("Increment ");
      }
      if ( navkey.readStatus(RDEC)) {
        Serial.print("Decrement ");
      }

      if ( navkey.readStatus(RMAX)) {
        Serial.print("Maximum threshold: ");
      }

      if ( navkey.readStatus(RMIN)) {
        Serial.print("Minimum threshold: ");
      }

      if ( navkey.readStatus(PUSHR)) {
        Serial.println("Push button Released");
      }

      if ( navkey.readStatus(PUSHP)) {
      }

      if ( navkey.readStatus(PUSHD)) {
        Serial.println("Double push!");   
      }
```


### uint8_t readStatus(void)

Return the status of the register **ESTATUS**



### bool readInt2(uint8_t s)
Must be called after **updateStatus()**, this method is used for checking if some event occurred on the secondary interrupt status **I2STATUS** register.
Return value is **true** in case of the event occurred, otherwise is **false**
Possible parameters are:

| Parameter   | Description   |
|:-----------:|:-------------|
| GP1_POS  | Positive edge on the GP1 pin |
| GP1_NEG  | Negative edge on the GP1 pin |
| GP2_POS  | Positive edge on the GP2 pin |
| GP2_NEG  |Negative edge on the GP2 pin |
| GP3_POS  | Positive edge on the GP3 pin|
| GP3_NEG  |Negative edge on the GP3 pin |
| FADE_INT |Fade process finished   |

#### Example:
```C++
 if ( navkey.updateStatus() == true) {
      if ( navkey.readInt2(GP1_POS)) {
        Serial.print("GP1 positive edge");
      }
      if ( navkey.readInt2(GP1_NEG)) {
        Serial.print("GP1 negative edge ");
      }

      if ( navkey.readInt2(GP2_POS)) {
          Serial.print("GP2 positive edge");
      }

      if ( navkey.readInt2(GP2_NEG)) {
        Serial.print("GP2 negative edge ");
      }

      if ( navkey.readInt2(GP3_POS)) {
        Serial.print("GP3 positive edge");
      }

      if ( navkey.readInt2(GP3_NEG)) {
        Serial.print("GP3 negative edge ");
      }

      if ( navkey.readInt2(FADE_INT)) {
        Serial.println("Fade process finished");   
      }
```

### uint8_t readInt2(void)

Return the status of the register **I2STATUS**

### bool readFadeStatus(uint8_t s)

When this function is called, it performs a I2C reading.
This function return **true** when the fade running, otherwise return **false**

| Parameter   | Description   |
|:-----------:|:-------------|
| FADE_GP1  | Fade process status of the GP1 |
| FADE_GP2  | Fade process status of the GP2 |
| FADE_GP3  | Fade process status of the GP3 |


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

### uint8_t readFadeGP(void)
Return the value of the FADEGP register. 

### uint8_t readEEPROM(uint8_t add)
Return the value of the EEPROM register. 
This function automatically manage the setting of the first and second memory bank.

### uint8_t readEncoderByte(uint8_t reg)
Read a generic register of the I2C Encoder V2.
The input parameter is the address of the register.

### int16_t readEncoderInt(uint8_t reg)
Read a generic register of the I2C Encoder V2, in 16bit format.
The input parameter is starting  address of the registers. 

###  int32_t readEncoderLong(uint8_t reg)
Read a generic register of the I2C Encoder V2, in 32bit format.
The input parameter is starting  address of the registers. 

### uint8_t readIDCode(void)
Return the ID code of the I2C NavKey, it's a fixed number 0x5B

### uint8_t readVersion(void)
Return the version of the  I2C NavKey.


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

### void writeDoublePushPeriod(uint8_t dperiod)
Write the DPPERIOD register.

### void writeFadeGP(uint8_t fade)
Write the FADEGP register.

### void writeEEPROM(uint8_t add, uint8_t data)
Write the EEPROM memory.
The input parameter *add* is the address. This method automatically change the first or the second bank.
The input parameter *data* is the data taht will be written.

