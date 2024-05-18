#include "lcd.h"

#include "twi.h"
#include "utils.h"
#include "systick.h"

/* Private Constants */

#define LCD_CURSOR_START_FIRST_LINE			(0x80)
#define LCD_CURSOR_START_SECOND_LINE		(0xC0)

static const uint8_t LCD_BIG_NUMBER_CODES[8][8] = {
    {
        0b00000001,//0
        0b00000001,
        0b00000001,
        0b00000001,
        0b00000001,
        0b00000001,
        0b00011111,
        0b00000000,
    },
    {
        0b00011111,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00000000,
    },
    {
        0b00011111,
        0b00000001,
        0b00000001,
        0b00000001,
        0b00000001,
        0b00000001,
        0b00000001,
        0b00000000,
    },
    {
        0b00000001,
        0b00000001,
        0b00000001,
        0b00000001,
        0b00000001,
        0b00000001,
        0b00000001,
        0b00000000,
    },
    {
        0b00011111,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00011111,
        0b00000000,
    },
    {
        0b00011111,
        0b00000001,
        0b00000001,
        0b00000001,
        0b00000001,
        0b00000001,
        0b00011111,
        0b00000000,
    },
    {
        0b00011111,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
    },
    {
        0b00011111,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00011111,
        0b00000000
    }
};

static const uint8_t LCD_BIG_NUMBER_COMMANDS[10][4] = {
		{0x01, 0x02, 0x4C, 0x00},
		{0x20, 0x7C, 0x20, 0x7C},
		{0x04, 0x05, 0x4C, 0x5F},
		{0x06, 0x05, 0x5F, 0x00},
		{0x4C, 0x00, 0x20, 0x03},
		{0x07, 0x04, 0x5F, 0x00},
		{0x07, 0x04, 0x4C, 0x00},
		{0x06, 0x02, 0x20, 0x03},
		{0x07, 0x05, 0x4C, 0x00},
		{0x07, 0x05, 0x20, 0x03} 
};

/* General Functions */

bool LCD_Init(const lcd_t *lcd) {    
    /* Wait until voltage stabilizes */
    delay_ms(50);

    /* Start in 8-bit mode [First try] */
	if(!LCD_WriteNibble(lcd, LCD_8BIT_FUNCTION_CMD)) {
        return false;
    }
	delay_us(4100);

    /* Start in 8-bit mode [Second try] */
	if(!LCD_WriteNibble(lcd, LCD_8BIT_FUNCTION_CMD)) {
        return false;
    }
	delay_us(100);

    /* Start in 8-bit mode [Third try] */
	if(!LCD_WriteNibble(lcd, LCD_8BIT_FUNCTION_CMD)) {
        return false;
    }
    delay_us(100);

	/* Set 4-bit mode */
	if(!LCD_WriteNibble(lcd, LCD_4BIT_FUNCTION_CMD)) {
        return false;
    }
    delay_us(100);

	/* Set font size and number of lines  */
	if(!LCD_Command(lcd, LCD_DEFAULT_FUNCTION_CMD)) {
        return false;
    }
    delay_us(53);

	/* Turn the display on with no cursor or blinking */
	if(!LCD_Command(lcd, LCD_DEFAULT_CONTROL_CMD)) {
        return false;
    }
    delay_us(53);

	/* Clear the Display */
	if(!LCD_Clear(lcd)) {
        return false;
    }

    /* Setup the cursor to move from left to right */
	if(!LCD_Command(lcd, LCD_DEFAULT_MODE_CMD)) {
        return false;
    }
    delay_us(53);

    return true;
}

bool LCD_Command(const lcd_t *lcd, uint8_t command) {
    return LCD_WriteNibble(lcd, LCD_HIGH_NIBBLE(command))
        && LCD_WriteNibble(lcd, LCD_LOW_NIBBLE(command));
}

bool LCD_Write(const lcd_t *lcd, uint8_t data) {
    return LCD_WriteNibble(lcd, LCD_HIGH_NIBBLE(data) | LCD_RS) 
        && LCD_WriteNibble(lcd, LCD_LOW_NIBBLE(data) | LCD_RS);
}

bool LCD_Print(const lcd_t *lcd, const char *str) {
    for(int i = 0; str[i] != '\0'; i++) {
        if(!LCD_Write(lcd, str[i])) {
            return false;
        }
    }

    return true;
}

/* Interface Functions */

bool LCD_Send(const lcd_t *lcd, uint8_t data) {   
    uint8_t expander_data = data | LCD_SET_BL(lcd->backlight);
    
    return TWI_WriteData(lcd->bus, lcd->address, 0, 0, &expander_data, 1);
}

bool LCD_WriteNibble(const lcd_t *lcd, uint8_t data) {
    return LCD_Send(lcd, data)
        && LCD_EnablePulse(lcd, data);
}

bool LCD_EnablePulse(const lcd_t *lcd, uint8_t data) {
    if(!LCD_Send(lcd, (data | LCD_EN))) {
        return false;
    }
	delay_us(1);

	if(!LCD_Send(lcd, (data & ~LCD_EN))) {
        return false;
    }
	delay_us(50);
    
    return true;
}

/* Specific Commands */

bool LCD_Backlight(const lcd_t *lcd) {
    return LCD_Send(lcd, 0);
}

bool LCD_Clear(const lcd_t *lcd) {
    if(!LCD_Command(lcd, LCD_CLEARDISPLAY_CMD)) {
        return false;
    }
	delay_ms(2);

    return true;
}

bool LCD_Home(const lcd_t *lcd) {
    if(!LCD_Command(lcd, LCD_RETURNHOME_CMD)) {
        return false;
    }
	delay_ms(2);

    return true;
}

bool LCD_SetCursor(const lcd_t *lcd, uint32_t col, uint32_t row) {
    uint8_t cursor_address = (row == 0) ? LCD_CURSOR_START_FIRST_LINE : LCD_CURSOR_START_SECOND_LINE;
    return LCD_Command(lcd, LCD_SETDDRAMADDR_CMD | (cursor_address + col));
}

bool LCD_CreateChar(const lcd_t *lcd, uint8_t char_address, const uint8_t* charmap) {
	if(!LCD_Command(lcd, LCD_SETCGRAMADDR_CMD | (char_address << 3))) {
        return false;
    }
	for (int i = 0; i < 8; i++) {
		if(!LCD_Write(lcd, charmap[i])) {
            return false;
        }
	}
    return true;
}

bool LCD_CreateBigNumbers(const lcd_t *lcd) {
	for (int i = 0, j = 0; i < 8; i++, j++) {
		if(!LCD_CreateChar(lcd, i, LCD_BIG_NUMBER_CODES[j])) {
            return false;
        }
	}
    return true;
}

bool LCD_WriteBigNumber(const lcd_t *lcd, uint8_t col, uint8_t number) {
    return LCD_SetCursor(lcd, col, 0)
        && LCD_Write(lcd, LCD_BIG_NUMBER_COMMANDS[number][0])
        && LCD_Write(lcd, LCD_BIG_NUMBER_COMMANDS[number][1])
        && LCD_SetCursor(lcd, col, 1)
        && LCD_Write(lcd, LCD_BIG_NUMBER_COMMANDS[number][2])
        && LCD_Write(lcd, LCD_BIG_NUMBER_COMMANDS[number][3]);
}

bool LCD_ScrollLeft(const lcd_t *lcd) {
    return LCD_Command(lcd, LCD_CURSORSHIFT_CMD | LCD_DISPLAYMOVE_FLAG | LCD_MOVELEFT_FLAG);
}

bool LCD_ScrollRight(const lcd_t *lcd) {
	return LCD_Command(lcd, LCD_CURSORSHIFT_CMD | LCD_DISPLAYMOVE_FLAG | LCD_MOVERIGHT_FLAG);
}

bool LCD_TurnOff(const lcd_t *lcd) {
	return LCD_Command(lcd, LCD_DEFAULT_CONTROL_CMD & ~LCD_DISPLAYON_FLAG);
}

bool LCD_TurnOn(const lcd_t *lcd) {
	return LCD_Command(lcd, LCD_DEFAULT_CONTROL_CMD | LCD_DISPLAYON_FLAG);
}

bool LCD_HideCursor(const lcd_t *lcd) {
	return LCD_Command(lcd, LCD_DEFAULT_CONTROL_CMD & ~LCD_CURSORON_FLAG);
}

bool LCD_ShowCursor(const lcd_t *lcd) {
	return LCD_Command(lcd, LCD_DEFAULT_CONTROL_CMD | LCD_CURSORON_FLAG);
}

bool LCD_NoBlink(const lcd_t *lcd) {
	return LCD_Command(lcd, LCD_DEFAULT_CONTROL_CMD & ~LCD_BLINKON_FLAG);
}

bool LCD_Blink(const lcd_t *lcd) {
	return LCD_Command(lcd, LCD_DEFAULT_CONTROL_CMD | LCD_BLINKON_FLAG);
}

bool LCD_LeftToRight(const lcd_t *lcd) {
	return LCD_Command(lcd, LCD_DEFAULT_MODE_CMD | LCD_CURSORINCREMENT_FLAG);
}

bool LCD_RightToLeft(const lcd_t *lcd) {
	return LCD_Command(lcd, LCD_DEFAULT_MODE_CMD & ~LCD_CURSORINCREMENT_FLAG);
}

bool LCD_AutoScroll(const lcd_t *lcd) {
	return LCD_Command(lcd, LCD_DEFAULT_MODE_CMD | LCD_ENTRYSHIFTINCREMENT_FLAG);
}

bool LCD_NoAutoScroll(const lcd_t *lcd) {
	return LCD_Command(lcd, LCD_DEFAULT_MODE_CMD & ~LCD_ENTRYSHIFTINCREMENT_FLAG);
}
