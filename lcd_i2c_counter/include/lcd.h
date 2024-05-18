#ifndef _LCD_H_
#define _LCD_H_

#include <stdint.h>
#include <sam3x8e.h>
#include <stdbool.h>

/* Data Type */

/*          Bit/Pin mappings        */
/* I2C Data D7-D6-D5-D4-D3-D2-D1-D0 */
/* LCD Pins D7-D6-D5-D4-BL-EN-RW-RS */

typedef struct lcd_t {
    Twi *bus;
    uint8_t address;
    bool backlight;
} lcd_t;

/* Public Constants */

#define LCD_NXP_EXPANSOR_ADDRESS        (0x27)
#define LCD_TI_EXPANSOR_ADDRESS         (0x3F)

#define LCD_BL                          (1U << 3)
#define LCD_EN                          (1U << 2)
#define LCD_RW                          (1U << 1)
#define LCD_RS                          (1U << 0)

/* Commands */

#define LCD_CLEARDISPLAY_CMD            (0x01)
#define LCD_RETURNHOME_CMD              (0x02)
#define LCD_ENTRYMODESET_CMD            (0x04)
#define LCD_DISPLAYCONTROL_CMD          (0x08)
#define LCD_CURSORSHIFT_CMD             (0x10)
#define LCD_FUNCTIONSET_CMD             (0x20)
#define LCD_SETCGRAMADDR_CMD            (0x40)
#define LCD_SETDDRAMADDR_CMD            (0x80)

/* Flags for display entry mode */

#define LCD_CURSORDECREMENT_FLAG        (0x00)
#define LCD_CURSORINCREMENT_FLAG        (0x02)
#define LCD_ENTRYSHIFTINCREMENT_FLAG    (0x01)
#define LCD_ENTRYSHIFTDECREMENT_FLAG    (0x00)

/* Flags for display on/off control */

#define LCD_DISPLAYON_FLAG              (0x04)
#define LCD_DISPLAYOFF_FLAG             (0x00)
#define LCD_CURSORON_FLAG               (0x02)
#define LCD_CURSOROFF_FLAG              (0x00)
#define LCD_BLINKON_FLAG                (0x01)
#define LCD_BLINKOFF_FLAG               (0x00)

/* Flags for display/cursor shift */

#define LCD_DISPLAYMOVE_FLAG            (0x08)
#define LCD_CURSORMOVE_FLAG             (0x00)
#define LCD_MOVERIGHT_FLAG              (0x04)
#define LCD_MOVELEFT_FLAG               (0x00)
 
/* Flags for function set */

#define LCD_8BITMODE_FLAG               (0x10)
#define LCD_4BITMODE_FLAG               (0x00)
#define LCD_2LINE_FLAG                  (0x08)
#define LCD_1LINE_FLAG                  (0x00)
#define LCD_5x10DOTS_FLAG               (0x04)
#define LCD_5x8DOTS_FLAG                (0x00)

/* Default Commands + Flags */

#define LCD_8BIT_FUNCTION_CMD           (LCD_FUNCTIONSET_CMD    \
                                        | LCD_8BITMODE_FLAG)

#define LCD_4BIT_FUNCTION_CMD           (LCD_FUNCTIONSET_CMD    \
                                        | LCD_4BITMODE_FLAG)

#define LCD_DEFAULT_FUNCTION_CMD        (LCD_FUNCTIONSET_CMD    \
                                        | LCD_4BITMODE_FLAG     \
                                        | LCD_2LINE_FLAG        \
                                        | LCD_5x8DOTS_FLAG)

#define LCD_DEFAULT_CONTROL_CMD         (LCD_DISPLAYCONTROL_CMD \
                                        | LCD_DISPLAYON_FLAG    \
                                        | LCD_CURSOROFF_FLAG    \
                                        | LCD_BLINKOFF_FLAG)

#define LCD_DEFAULT_MODE_CMD            (LCD_ENTRYMODESET_CMD   \
                                        | LCD_CURSORINCREMENT_FLAG    \
                                        | LCD_ENTRYSHIFTDECREMENT_FLAG)

/* Public Macros */

#define LCD_MASK                        (0xF0)
#define LCD_HIGH_NIBBLE(data)           (data & LCD_MASK)
#define LCD_LOW_NIBBLE(data)            ((data << 4) & LCD_MASK)
#define LCD_SET_BL(_bl)                 (_bl << 3)

#define LCD_RIGHT_JUSTIFY(lcd_handle)   (LCD_AutoScroll(lcd_handle))
#define LCD_LEFT_JUSTIFY(lcd_handle)    (LCD_NoAutoScroll(lcd_handle))

/* General Functions */

bool LCD_Init(const lcd_t *lcd);

bool LCD_Command(const lcd_t *lcd, uint8_t command);

bool LCD_Write(const lcd_t *lcd, uint8_t data);

bool LCD_Print(const lcd_t *lcd, const char *str);

/* Interface Functions */

bool LCD_Send(const lcd_t *lcd, uint8_t data);

bool LCD_WriteNibble(const lcd_t *lcd, uint8_t data);

bool LCD_EnablePulse(const lcd_t *lcd, uint8_t data);

/* Specific Commands */

bool LCD_Backlight(const lcd_t *lcd);

bool LCD_Clear(const lcd_t *lcd);

bool LCD_Home(const lcd_t *lcd);

bool LCD_SetCursor(const lcd_t *lcd, uint32_t col, uint32_t row);

bool LCD_CreateChar(const lcd_t *lcd, uint8_t char_address, const uint8_t* charmap);

bool LCD_CreateBigNumbers(const lcd_t *lcd);

bool LCD_WriteBigNumber(const lcd_t *lcd, uint8_t col, uint8_t number);

bool LCD_ScrollLeft(const lcd_t *lcd);

bool LCD_ScrollRight(const lcd_t *lcd);

bool LCD_TurnOff(const lcd_t *lcd);

bool LCD_TurnOn(const lcd_t *lcd);

bool LCD_HideCursor(const lcd_t *lcd);

bool LCD_ShowCursor(const lcd_t *lcd);

bool LCD_NoBlink(const lcd_t *lcd);

bool LCD_Blink(const lcd_t *lcd);

bool LCD_LeftToRight(const lcd_t *lcd);

bool LCD_RightToLeft(const lcd_t *lcd);

bool LCD_AutoScroll(const lcd_t *lcd);

bool LCD_NoAutoScroll(const lcd_t *lcd);

#endif
