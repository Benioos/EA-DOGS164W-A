# Universal EA-DOGS164W-A Driver (MCAL-Based)

This is a portable, hardware-agnostic C driver for the **EA-DOGS164W-A** (4x16 LCD) display controller based on **ST7036**. 
This library provides a way of controlling the LCD module over SPI, sending commands, printing strings, or selecting font sets.

---

<img src="https://mm.digikey.com/Volume0/opasdata/d220001/medias/images/328/EA-DOGS164W-A.jpg" alt="EA-DOGS164W-A LCD Display Module" width="260">

## ✨ Implemented Features

### 🔧 Core API 

- `LCD_Write` → Write a byte (command or data) over SPI
- `LCD_Command` → Send a command byte to the display
- `LCD_Data` → Send a character byte to the display
- `reverse_nibble` → Swaps bit order of a nibble (utility function)

<details>
<summary> Full reference </summary>

| Core API | Description | Example |
| :--- | :--- | :--- | 
|  `LCD_Write` | Write a byte over SPI (0 = Command, 1 = Data) |  ```LCD_Write(0, 0x01);``` | 
|  `LCD_Command` | Send a command byte |  ```LCD_Command(0x01); // Clear Display``` | 
|  `LCD_Data` | Send a character byte |  ```LCD_Data('A');``` | 
|  `reverse_nibble` | Reverse bits of a 4-bit nibble |  ```uint8_t rev = reverse_nibble(0x0E);``` | 

</details>

### ⚙️ Display Initialisation

- `LCD_Init` → Initialize SPI pins, reset cycle, and ST7036 startup parameters
- `SPI_Select_LCD` → Pull CS line LOW
- `SPI_Deselect_LCD` → Pull CS line HIGH
- `LCD_Select_ROM` → Select internal character ROM (ST7036 font bank)

<details>
<summary> Full reference </summary>

| Display Initialization| Description | Example/Available Modes |
| :--- | :--- | :--- |
| `LCD_Init` | Initialize SPI, GPIOs, and run initialization sequence | ```LCD_Init();``` |
| `SPI_Select_LCD` | Assert Chip Select pin (CS LOW) | ```SPI_Select_LCD();``` |
| `SPI_Deselect_LCD` | Deassert Chip Select pin (CS HIGH) | ```SPI_Deselect_LCD();``` |
| `LCD_Select_ROM` | Select active character set ROM | `ROM_A`<br> `ROM_B`<br> `ROM_C` |

</details>

### 📡 Display Operations & Text Rendering

- `LCD_Print` → Send a null-terminated string to the display
- `LCD_SetCursor` → Move cursor position (row, column)

<details>
<summary> Full reference </summary>

| Display Operation | Description | Example/Available Modes |
| :--- | :--- | :--- |
| `LCD_Print` | Print string at current cursor position | ```LCD_Print("Hello World!");``` |
| `LCD_SetCursor` | Set cursor position on grid (4 lines x 16 chars) | ```LCD_SetCursor(0, 0); // Row 0, Col 0```<br> ```LCD_SetCursor(3, 15); // Row 3, Col 15``` |

</details>

---

## 💡 SPI Tested configuration

The SPI was configured as follows:

- **Frame Format :** Motorola
- **Mode :** Full-Duplex Master  
- **CPOL :** Low   
- **CPHA :** 2 Edge  
- **First Bit :** MSB First

## 🚀 How to Port the Driver to Another Microcontroller

Porting this driver to a new platform requires **zero modifications** to the `DOG_LCD.c` file. Everything happens in the header configuration (`DOG_LCD.h`).

Simply adjust the GPIO, SPI handle, and hardware mapping declarations to match your microcontroller platform:

```c
#ifndef DOG_LCD_H
#define DOG_LCD_H

#include "stm32l4xx_hal.h" // Adapt to your target MCU (e.g. esp_err.h, Arduino.h...)

// Pin Definitions
#define LCD_CS_PORT    GPIOA
#define LCD_CS_PIN     GPIO_PIN_2
#define LCD_RST_PORT   GPIOA
#define LCD_RST_PIN    GPIO_PIN_3

// Prototypes
uint8_t reverse_nibble(uint8_t nibble);
void LCD_Write(uint8_t is_data, uint8_t value);
void LCD_Command(uint8_t cmd);
void LCD_Data(uint8_t data);
void LCD_Init(void);
void LCD_Print(char *str);
void LCD_SetCursor(uint8_t row, uint8_t col);
void SPI_Select_LCD(void);
void SPI_Deselect_LCD(void);
void LCD_Select_ROM(uint8_t rom_id);

#endif /* DOG_LCD_H */
