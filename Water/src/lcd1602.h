/* Created by The Vien Dep Trai */

void lcd_send (uint8_t value, uint8_t rs);
void lcd_command (uint8_t command);
void lcd_data (uint8_t data);
void lcd_init (uint8_t cursor, uint8_t blink);
void lcd_set_cursor (uint8_t x, uint8_t y);
void lcd_create_char (uint8_t addr, uint8_t *data);
void clearDisplay();
