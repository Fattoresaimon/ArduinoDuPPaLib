# I2C Encoder Mini Arduino Library
--------------------------------------------------------------------------------

## Introduction

Here you can find the library description of the [I2C Encoder Mini](https://github.com/Fattoresaimon/I2CEncoderMini) for the Arduino IDE.
For more details of the functionality of the board, please read the Datasheet on GitHub 

The I2C Encoder Mini is available on [Tindie!](https://www.tindie.com/products/saimon/i2c-encoder-mini/)

## Initialization of the class

The library makes available the class **i2cEncoderMiniLib**
To initialize the library, you have to declare an instance of the class **i2cEncoderMiniLib** for each encoders.
For example:

``` C++
i2cEncoderMiniLib encoder(0x30);
```
Declaration of one encoder with the address 0x30. The jumper A4 and A5 are shorted.

``` C++
i2cEncoderMiniLib encoder(0b0110000);
```
Declaration of the same encoder but in binary format.

```C++
i2cEncoderMiniLib encoder1(0x30);
i2cEncoderMiniLib encoder2(0x32);
```
Declaration of two encoders with the address 0x30 and 0x34 in two separated variable.

```C++
i2cEncoderMiniLib encoder[2] = { i2cEncoderMiniLib(0x30), i2cEncoderMiniLib(0x34)};
```
Declaration of an array of the two encoders with the address 0x30 and 0x34.

## Parameters

Each class has also a public variable called **id** that is used for setting a custom id to each class.

```C++
i2cEncoderMiniLib encoder(0x30);
encoder.id=1 
```
The **id** can be useful when you declare an array of i2cEncoderMiniLib class. The **id** can be used as a index in the loops.

## Callback Configuration

This library support the callback functionality.
There is the possibility to link a function to a specific interrupt of the I2C Encoder V2, in this way the function is automatically called when the I2C Encoder V2 generates an interrupts.

A callback function must be declared as the following:

```C++
void NAME_OF_THE_FUNCTION(i2cEncoderMiniLib* obj)
```

The argument **i2cEncoderMiniLib* obj**  is the pointer to the class that called the method.

There are 16 possible events:

| Event   | Description   |
|:-----------:|:----------------------------------|
| onButtonRelease | Encoder push button is released |
| onButtonPush | Encoder push button is pushed |
| onButtonDoublePush | Encoder push button is double pushed |
| onButtonLongPush | Encoder push button is long press|
| onIncrement | The counter value is incremented |
| onDecrement | The counter value is decremented |
| onChange | The counter value is incremented or decremented |
| onMax | The counter value reach the maximum threshold |
| onMin | The counter value reach the minimum threshold |
| onMinMax | The counter value reach the maximum or minimum threshold |


#### Examples:

```C++
i2cEncoderMiniLib Encoder(0x61); // Class declaration

...

// Simple callback that ist's called when the encoder is rotated
void encoder_change(i2cEncoderMiniLib* obj) {
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
| WRAP_ENABLE | Wrap enable. When the counter value reaches the CMAX+1, restart to the CMIN and vice versa |
| WRAP_DISABLE | Wrap disable. When the counter value reaches the CMAX or CMIN, the counter stops to increasing or decreasing |
| | |
| DIRE_LEFT | Rotate left side to increase the value counter |
| DIRE_RIGHT | Rotate right side to increase the value counter |
| | |
| IPUP_DISABLE | Disable the internal pull-up on the INT pin |
| IPUP_ENABLE | Enable the internal pull-up on the INT pin |
| | |
| RMOD_X4 | Encoder in X4 mode |
| RMOD_X2 | Encoder in X2 mode |
| RMOD_X1 | Encoder in X1 mode |
| | |
| RESET | Reset the board |



#### Examples:

```C++
encoder.begin(i2cEncoderMiniLib::WRAP_DISABLE | i2cEncoderMiniLib::DIRE_LEFT | i2cEncoderMiniLib::IPUP_ENABLE | i2cEncoderMiniLib::RMOD_X1 );
```

Please remember to add the class name **i2cEncoderMiniLib::** before the parameter!

### void reset(void)

Reset of the board. 
In this command there is 10ms delay in order to make the board correctly restart.



## Configuration

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
 Encoder.writeInterruptConfig(i2cEncoderMiniLib::INT_2 | i2cEncoderMiniLib::RMIN | i2cEncoderMiniLib::RMAX | i2cEncoderMiniLib::RDEC | i2cEncoderMiniLib::RINC | i2cEncoderMiniLib::PUSHR); 
```

Please remember to add the class name **i2cEncoderMiniLib::** before the parameter!

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

### void writeDoublePushPeriod(uint8_t dperiod)

This method is used for setting the window period **DPPERIOD** of the double push of the rotary encoder switch. When the value is 0, the double push option is disabled.
The I2C encoder V2 will multiplies this value by 10 (value x10).

#### Examples:

```C++
encoder.writeDoublePushPeriod(50);  //Set a period for the double push of 500ms 
```

### void ChangeI2CAddress(uint8_t add)
This methods is used to change the I2C address of the device.
The changes will have effect after a reset or a power cycle of the board.

**Pay attention with this command, it's not reversible!**


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
      if ( Encoder.readStatus(i2cEncoderMiniLib::RINC)) {
        Serial.print("Increment ");
      }
      if ( Encoder.readStatus(i2cEncoderMiniLib::RDEC)) {
        Serial.print("Decrement ");
      }

      if ( Encoder.readStatus(i2cEncoderMiniLib::RMAX)) {
        Serial.print("Maximum threshold: ");
      }

      if ( Encoder.readStatus(i2cEncoderMiniLib::RMIN)) {
        Serial.print("Minimum threshold: ");
      }

      if ( Encoder.readStatus(i2cEncoderMiniLib::PUSHR)) {
        Serial.println("Push button Released");
      }

      if ( Encoder.readStatus(i2cEncoderMiniLib::PUSHP)) {
      }

      if ( Encoder.readStatus(i2cEncoderMiniLib::PUSHD)) {
        Serial.println("Double push!");   
      }
```

Please remember to add the class name **i2cEncoderMiniLib::** before the parameter!

### uint8_t readStatus(void)
Return the status of the register **ESTATUS**



## Reading methods

### int32_t readCounterLong(void)
Return the counter value in the format **int32_t**, by reading all the 4 bytes of the counter value registers.

### int16_t readCounterInt(void)
Return the counter value in the format **int16_t**, by reading the 2 LSB  of the counter value registers.
Useful when the counter register is between the values -32768 to 32767.

### int8_t readCounterByte(void)
Return the counter value in the format **int8_t**, by reading the LSB byte of the counter value register.
Useful when the counter register is between the values -128 to 127


### int32_t readMax(void)
Return the maximum threshold in format **int32_t**, bye reading all the 4 bytes of the counter Max.


### int32_t readMin(void)
Return the minimum threshold in format **int32_t**, by reading all the 4 byte of the counter Min.


### int32_t readStep(void)
Return the minimum threshold in format **int32_t**, by reading all the 4 bytes of the ISTEP registers.

### uint8_t readDoublePushPeriod(void)
Return the value of the DPPERIOD register. 

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

###  void writeMin(int32_t min)
Write the Min register with a  **int32_t** number. All of the 4 bytes are wrote.

###  void writeStep(int32_t step)
Write the increment step  with a  **int32_t** number. All of the 4 bytes are wrote.

### void writeDoublePushPeriod(uint8_t dperiod)
Write the DPPERIOD register.


### void writeEEPROM(uint8_t add, uint8_t data)
Write the EEPROM memory.
The input parameter *add* is the address. 
The input parameter *data* is the data that will be written.


