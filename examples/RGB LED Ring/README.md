# RGB LED Ring Arduino Library

--------------------------------------------------------------------------------

## Introduction

Here you can find the project  description of the [RGB LED Ring Small](https://github.com/Fattoresaimon/RGB_LED_Ring).
The RGB LED Ring is based on the driver ISSI [IS31FL3745](https://www.lumissil.com/assets/pdf/core/IS31FL3745_DS.pdf), for more functionality please check the datasheet of the driver.
At the moment the breathing effect are not implemented.

The RGB LED Ring is available on [DuPPa Store!](https://www.duppa.net/shop/rgb-led-ring/)

## Initialization of the class

The library makes available the class **LEDRing**
To initialize the library, you have to declare an instance of the class **LEDRing** for each LED Ring.
For example:

``` C++
LEDRing LEDRing(ISSI3745_SJ1 | ISSI3745_SJ5);
```
Declaration of the RGB LED Ring with the jumper SJ1 and SJ5 soldered.
The  RGB LED Ring v1.1 it have 8 Jumpers for setting the address, they are divided in 2 groups of 4 jumpers. Only 1 jumper per group should be soldered,  this make 16 possible combinations.
Possible combination are the following:

| Jumper first group | Jumper second group | I2C Address |
| ------------ | ------------ | :--: |
| ISSI3745_SJ1 | ISSI3745_SJ5 | 0x40 |
| ISSI3745_SJ2 | ISSI3745_SJ5 | 0x42 |
| ISSI3745_SJ3 | ISSI3745_SJ5 | 0x44 |
| ISSI3745_SJ4 | ISSI3745_SJ5 | 0x46 |
| ISSI3745_SJ1 | ISSI3745_SJ6 | 0x48 |
| ISSI3745_SJ2 | ISSI3745_SJ6 | 0x4A |
| ISSI3745_SJ3 | ISSI3745_SJ6 | 0x4C |
| ISSI3745_SJ4 | ISSI3745_SJ6 | 0x4E |
| ISSI3745_SJ1 | ISSI3745_SJ7 | 0x50 |
| ISSI3745_SJ2 | ISSI3745_SJ7 | 0x52 |
| ISSI3745_SJ3 | ISSI3745_SJ7 | 0x54 |
| ISSI3745_SJ4 | ISSI3745_SJ7 | 0x56 |
| ISSI3745_SJ1 | ISSI3745_SJ8 | 0x58 |
| ISSI3745_SJ2 | ISSI3745_SJ8 | 0x5A |
| ISSI3745_SJ3 | ISSI3745_SJ8 | 0x5C |
| ISSI3745_SJ4 | ISSI3745_SJ8 | 0x5E |



## Configuration

### void LEDRing_Reset(void)
Reset all the IS31FL3745 to the default state

### void LEDRing_Configuration(uint8_t conf)
This method write the register ad the address 00h of the IS31FL3745. Please refer the datasheet for further information.

### void LEDRing_GlobalCurrent(uint8_t conf)
This method write the register 01h and it set the LEDs current. 
It's possible to set up to 256 step from 0 to 0xFF. 
Higher value make the LEDs brighter.

### void LEDRing_SetScaling(uint8_t led_n, uint8_t scal);
Set the output current of each led. **n** is the LED number ( 1 to 144  ), **scal** is the scaling value (0 to 256)

### void LEDRing_SetScaling(uint8_t scal);
Set the output current of all LED. **scal** is the scaling value (0 to 256)

### void LEDRing_PULLUP_DOWN(uint8_t pull)
Configure the PULLUP and PULLDOWN resistor. You can refer to the IS31FL3745 datasheet at the 02h register for further information.

### uint8_t LEDRing_Temperature(void);
Get the temperature status register. Refer to the IS31FL3745 datasheet at the 24h register for further information.

### void LEDRing_PWM_MODE(void)
This method set all the LEDs in PWM mode.

### void LEDRing_ClearAll(void)
This method set the PWM value to 0 for each LED.

### void LEDRing_Set_RGB(uint8_t led_n, uint32_t color)
Set the RGB color for a specific LED.
**n** is the LED number ( 0 to 47 ), while the **color** is the RGB color in 24bit format.

#### Examples:

```C++
   LEDRing.LEDRing_Set_RGB(10, 0xFF9933);
```

### void LEDRing_Set_RED(uint8_t led_n, uint8_t color)
### void LEDRing_Set_GREEN(uint8_t led_n, uint8_t color)
### void LEDRing_Set_BLUE(uint8_t led_n, uint8_t color)

Set a specific color to a specific LED
**n** is the LED number ( 0 to 47 ), while the **color** is the  color in 8bit format.

#### Examples:

```C++
	LEDRing.LEDRing_Set_RED(4, 128);
	LEDRing.LEDRing_Set_GREEN(5, 200);
	LEDRing.LEDRing_Set_BLUE(6, 80);
```
