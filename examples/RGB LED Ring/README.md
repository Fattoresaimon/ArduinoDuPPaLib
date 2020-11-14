# RGB LED Ring Arduino Library

--------------------------------------------------------------------------------

## Introduction

Here you can find the library description of the [RGB LED Ring](https://github.com/Fattoresaimon/RGB_LED_Ring) for the Arduino IDE.
The RGB LED Ring is based on the driver ISSI [IS31FL3737 ](http://www.issi.com/WW/pdf/31FL3737.pdf), for more functionality please check the datasheet of the driver.
At the moment the breathing effect are not implemented.

The RGB LED Ring is available on [Tindie!]( https://www.tindie.com/products/20279/)

## Initialization of the class

The library makes available the class **LEDRing**
To initialize the library, you have to declare an instance of the class **LEDRing** for each LED Ring.
For example:

``` C++
LEDRing LEDRing(0x5A);
```
Declaration of the RGB LED Ring with the jumper 0x5A soldered


## Configuration

### void LEDRing_Reset(void)
Reset all the IS31FL3737 to the default state

### void LEDRing_Configuration(uint8_t conf)
This method write the register ad the address 00h of the IS31FL3737. Please refer the datasheet for further information.

### void LEDRing_GlobalCurrent(uint8_t conf)
This method write the register 01h and it set the LEDs current. 
It's possible to set up to 256 step from 0 to 0xFF. 
Higher value make the LEDs brighter.

### void LEDRing_PULLUP(uint8_t pull)
### void LEDRing_PULLDOWN(uint8_t pull)
Configure the PULLUP or PULLDOWN resistor.
This resistor are used for avoid the "ghost" effect in a matrix LED architecture.
The possible value are the following:

| PUR / PDR |  Value |
| :--: | ---- |
| 000 |  No pull-up resistor |
| 001 | 0.5kΩ |
| 010 | 1.0kΩ |
| 011 | 2.0kΩ |
| 100 | 4.0kΩ |
| 101 | 8.0kΩ |
| 110 | 16kΩ |
| 111 | 32kΩ |

### void LEDRing_EnableAllOutput(void)
This method enable all the LEDs, by default the LEDs are disabled.

### void LEDRing_DisableAllOutput(void)
This method disable all the LEDs.

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
