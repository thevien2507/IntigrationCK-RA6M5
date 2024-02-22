#include "hal_data.h"
#include "lcd1602.h"

void lcd_send (uint8_t value, uint8_t rs)
{
    R_BSP_PinAccessEnable();
    R_BSP_PinWrite(LCD_RS, rs);
    R_BSP_PinWrite(LCD_E, 1);
    R_BSP_PinWrite(LCD_D4, (value & 0x10) >> 4);
    R_BSP_PinWrite(LCD_D5, (value & 0x20) >> 5);
    R_BSP_PinWrite(LCD_D6, (value & 0x40) >> 6);
    R_BSP_PinWrite(LCD_D7, (value & 0x80) >> 7);
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MICROSECONDS);
    R_BSP_PinWrite(LCD_E, 0);
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MICROSECONDS);
    R_BSP_PinWrite(LCD_E, 1);
    R_BSP_PinWrite(LCD_D4, (value & 0x01));
    R_BSP_PinWrite(LCD_D5, (value & 0x02) >> 1);
    R_BSP_PinWrite(LCD_D6, (value & 0x04) >> 2);
    R_BSP_PinWrite(LCD_D7, (value & 0x08) >> 3);
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MICROSECONDS);
    R_BSP_PinWrite(LCD_E, 0);
    R_BSP_SoftwareDelay(40, BSP_DELAY_UNITS_MICROSECONDS);
    R_BSP_PinAccessDisable();
}

void lcd_command (uint8_t command)
{
    lcd_send(command, 0);
}

void lcd_data (uint8_t data)
{
    lcd_send(data, 1);
}

void lcd_init (uint8_t cursor, uint8_t blink)
{
    lcd_command(0x30);
    R_BSP_SoftwareDelay(4200, BSP_DELAY_UNITS_MICROSECONDS);
    lcd_command(0x30);
    lcd_command(0x28);
    lcd_command(0x08);
    lcd_command(0x01);
    R_BSP_SoftwareDelay(4200, BSP_DELAY_UNITS_MICROSECONDS);
    lcd_command(0x06);
    lcd_command(0x0C | (cursor << 1) | blink);
}

void lcd_set_cursor (uint8_t x, uint8_t y)
{
    if (y > 2)
        y = 2;
    if (x > 16)
        x = 16;
    lcd_command(0x80 | ((y - 1) << 6) | (x - 1));
}

void lcd_create_char (uint8_t addr, uint8_t *data)
{
    if (addr > 7)
        addr = 7;
    lcd_command (0x40 | addr << 3);
    for (uint8_t i = 0; i < 8; i ++)
    {
        lcd_data (data[i]);
    }
}

void clearDisplay(){
    lcd_command(0x01);
}
