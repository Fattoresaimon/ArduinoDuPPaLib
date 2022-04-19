# RGB LED Ring Small Arduino Library

--------------------------------------------------------------------------------

## Introduction

Here you can find the project  description of the [RGB LED Ring Small](https://github.com/Fattoresaimon/RGB_LED_Ring_Small).
The RGB LED Ring is based on the driver ISSI [IS31FL3746A](https://www.lumissil.com/assets/pdf/core/IS31FL3746A_DS.pdf), for more functionality please check the datasheet of the driver.
At the moment the breathing effect are not implemented.

The RGB LED Ring is available on [DuPPa Store!](https://www.duppa.net/shop/rgb-led-ring-small/)

## Initialization of the class

The library makes available the class **LEDRingSmall**
To initialize the library, you have to declare an instance of the class **LEDRingSmall** for each LED Ring.
For example:

``` C++
LEDRingSmall LEDRingSmall(ISSI3746_SJ2 | ISSI3746_SJ7);
```
Declaration of the RGB LED Ring with the jumper SJ2 and SJ7 soldered.
The  RGB LED Ring v1.1 it have 8 Jumpers for setting the address, they are divided in 2 groups of 4 jumpers. Only 1 jumper per group should be soldered,  this make 16 possible combinations.
Possible combination are the following:

| Jumper first group | Jumper second group | I2C Address |
| ------------ | ------------ | :--: |
| ISSI3746_SJ1 | ISSI3746_SJ5 | 0x60 |
| ISSI3746_SJ2 | ISSI3746_SJ5 | 0x62 |
| ISSI3746_SJ3 | ISSI3746_SJ5 | 0x64 |
| ISSI3746_SJ4 | ISSI3746_SJ5 | 0x66 |
| ISSI3746_SJ1 | ISSI3746_SJ6 | 0x68 |
| ISSI3746_SJ2 | ISSI3746_SJ6 | 0x6A |
| ISSI3746_SJ3 | ISSI3746_SJ6 | 0x6C |
| ISSI3746_SJ4 | ISSI3746_SJ6 | 0x6E |
| ISSI3746_SJ1 | ISSI3746_SJ7 | 0x70 |
| ISSI3746_SJ2 | ISSI3746_SJ7 | 0x72 |
| ISSI3746_SJ3 | ISSI3746_SJ7 | 0x74 |
| ISSI3746_SJ4 | ISSI3746_SJ7 | 0x76 |
| ISSI3746_SJ1 | ISSI3746_SJ8 | 0x78 |
| ISSI3746_SJ2 | ISSI3746_SJ8 | 0x7A |
| ISSI3746_SJ3 | ISSI3746_SJ8 | 0x7C |
| ISSI3746_SJ4 | ISSI3746_SJ8 | 0x7E |



## Configuration

### void LEDRingSmall_Reset(void)
Reset all the IS31FL3746A to the default state

### void LEDRing_Configuration(uint8_t conf)
This method write the register ad the address 50h of the IS31FL3746A. Please refer the datasheet for further information.

### void LEDRing_GlobalCurrent(uint8_t conf)
This method write the register 51h and it set the LEDs current. 
It's possible to set up to 256 step from 0 to 0xFF. 
Higher value make the LEDs brighter.

### void LEDRing_SetScaling(uint8_t led_n, uint8_t scal);
Set the output current of each single color  led. **n** is the LED number ( 1 to 72  ), **scal** is the scaling value (0 to 256)

### void LEDRing_SetScaling(uint8_t scal);
Set the output current of all LED. **scal** is the scaling value (0 to 256)

### void LEDRing_PULLUP_DOWN(uint8_t pull)
Configure the PULLUP and PULLDOWN resistor. You can refer to the IS31FL3746A datasheet at the 52h register for further information.

### void LEDRingSmall_SpreadSpectrum(uint8_t spread);
Enable and configure the spread spectrum functionality. You can refer to the IS31FL3746A datasheet at the 60h register for further information.

### void LEDRingSmall_PWMFrequencyEnable(uint8_t PWMenable);
Enable or Disable the PWM output. Write 1 for enable the PWM output, write 0 for disable the PWM output.

### void LEDRingSmall_PWMFrequencySetting(uint8_t pwmfreq);
Configure the PWM frequency. It's possible to configure the PWM frequency from 29kHz to 453Hz. You can refer to the IS31FL3746A datasheet at the E2h register for further information.

### uint8_t LEDRing_Temperature(void);
Get the temperature status register. Refer to the IS31FL3746A datasheet at the 24h register for further information.

### void LEDRing_PWM_MODE(void)
This method set all the LEDs in PWM mode.

### void LEDRing_ClearAll(void)
This method set the PWM value to 0 for each LED.

### void LEDRing_Set_RGB(uint8_t led_n, uint32_t color)
Set the RGB color for a specific LED.
**n** is the LED number ( 0 to 23 ), while the **color** is the RGB color in 24bit format.

#### Examples:

```C++
   LEDRingSmall.LEDRingSmall_Set_RGB(10, 0xFF9933);
```

### void LEDRing_Set_RED(uint8_t led_n, uint8_t color)
### void LEDRing_Set_GREEN(uint8_t led_n, uint8_t color)
### void LEDRing_Set_BLUE(uint8_t led_n, uint8_t color)

Set a specific color to a specific LED
**n** is the LED number ( 0 to 24 ), while the **color** is the  color in 8bit format.

#### Examples:

```C++
	LEDRingSmall.LEDRingSmall_Set_RED(4, 128);
	LEDRingSmall.LEDRingSmall_Set_GREEN(5, 200);
	LEDRingSmall.LEDRingSmall_Set_BLUE(6, 80);
```
