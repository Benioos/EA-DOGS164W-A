#include "doglcd.h"
extern SPI_HandleTypeDef hspi1;

uint8_t reverse_nibble(uint8_t nibble)
{
    uint8_t res = 0;
    if (nibble & 0x01) res |= 0x80;
    if (nibble & 0x02) res |= 0x40;
    if (nibble & 0x04) res |= 0x20;
    if (nibble & 0x08) res |= 0x10;
    return res;
}

void LCD_Write(uint8_t is_data, uint8_t value)
{
    uint8_t buffer[3];

    // Command Mode : 0xF8 | Data Mode : 0xFA
    buffer[0] = is_data ? 0xFA : 0xF8;

    // 2. Séparation de la valeur en 2 octets (LSB nibble puis MSB nibble, alignés à gauche)
    buffer[1] = reverse_nibble(value & 0x0F);        // Bits 0-3
    buffer[2] = reverse_nibble((value >> 4) & 0x0F); // Bits 4-7

    HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, buffer, 3, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);
    HAL_Delay(1);
}

void LCD_Command(uint8_t cmd)
{
    LCD_Write(0, cmd);
}

void LCD_Data(uint8_t data)
{
    LCD_Write(1, data);
}

void LCD_Init(void)
{
    HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET);
    HAL_Delay(50);

    HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);
    HAL_Delay(5);

    LCD_Command(0x3A); // Function Set: 8-bit data, extension RE=1, IS=0
    LCD_Command(0x09); // Extended function set: 4 lines display
    LCD_Command(0x06); // Entry mode set: bottom view, cursor auto-increment
    LCD_Command(0x1E); // Bias setting: BS1=1

    LCD_Command(0x39); // Function Set: 8-bit data, extension RE=0, IS=1
    LCD_Command(0x1B); // Internal OSC: BS0=1 -> Bias=1/6
    LCD_Command(0x6C); // Follower control: Divider on and set value
    LCD_Command(0x54); // Power control: Booster on and set contrast (C5, C4)
    LCD_Command(0x72); // Contrast Set: Set contrast (C3-C0)

    LCD_Command(0x38); // Function Set: 8-bit data, extension RE=0, IS=0
    LCD_Command(0x0C); // Display On: Écran allumé, curseur invisible (0x0F pour l'activer)

    // Effacer l'écran
    LCD_Command(0x01);
    HAL_Delay(5);
}

void LCD_Print(char *str)
{
    while (*str) {
        LCD_Data((uint8_t)*str++);
    }
}

void LCD_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t row_offsets[] = {0x00, 0x20, 0x40, 0x60};
    if (row > 3) row = 3;
    if (col > 15) col = 15;

    LCD_Command(0x80 + row_offsets[row] + col);
}

void SPI_Select_LCD(void)
{
    while (HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_BUSY);
    hspi1.Instance->CR1 |= (SPI_CR1_CPOL | SPI_CR1_CPHA);
    HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET);
}

void SPI_Deselect_LCD(void)
{
    HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);
}

void LCD_Select_ROM(uint8_t rom_id)
{

    LCD_Command(0x3A);

    LCD_Command(0x72);
    if(rom_id == 'A')      LCD_Data(0x00);
        else if(rom_id == 'B') LCD_Data(0x04);
        else if(rom_id == 'C') LCD_Data(0x0C);

    LCD_Command(0x39);
}


