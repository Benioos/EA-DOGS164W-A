#ifndef DOG_LCD_H
#define DOG_LCD_H

#include "stm32l4xx_hal.h" // Adapté à votre STM32L4

// Définitions des broches
#define LCD_CS_PORT    GPIOA
#define LCD_CS_PIN     GPIO_PIN_2
#define LCD_RST_PORT   GPIOA
#define LCD_RST_PIN     GPIO_PIN_3

// Prototypes des fonctions (Déclarations seules)
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
