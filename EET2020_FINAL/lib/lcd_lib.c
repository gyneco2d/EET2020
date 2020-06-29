/*
 * lcd_lib.c
 *
 * Created: 2020/06/29 2:07:34
 *  Author: gyneco2d
 */ 

#include <atmel_start.h>
#include <stdio.h>
#include <string.h>
#include "i2c_lib.h"
#include "lcd_lib.h"
#include "beep.h"

static int double_height = 0;

int getHeight()
{
	return double_height;
}

void set_table(instruction_table table)
{
	// Function Set: 0b001/DL/N/DH/0/IS
	// DL=1(8bit), N=!double_height, DH=double_height, IS=0(normal instruction)
	uint8_t param = double_height ? 0b00110100 : 0b00111000;
	param = table == NORMAL ? param : param | 0x01;

	if (i2c_write_single(LCD_ADDR, INSTRUCT, param)) {
		puts("error occurred");
	}
	delay_us(20);
}

void cursor_init()
{
	// Entry Mode Set: 0b000001/I-D/S
	// I/D=1, S=1
	if (i2c_write_single(LCD_ADDR, INSTRUCT, 0b00000110)) {
		puts("error occurred");
	}
	delay_us(20);

	// Return Home
	if (i2c_write_single(LCD_ADDR, INSTRUCT, 0b00000010)) {
		puts("error occurred");
	}
	delay_us(800);
}

void lcd_init()
{
	// Function Set: 0b001/DL/N/DH/0/IS
	// DL=1(8bit), N=1(2-line), DH=0(not double-height), IS=0(normal instruction)
	if (i2c_write_single(LCD_ADDR, INSTRUCT, 0b00111000)) {
		puts("error occurred");
	}
	delay_us(20);

	// Function Set: 0b001/DL/N/DH/0/IS
	// DL=1(8bit), N=1(2-line), DH=0(not double-height), IS=1(extension instruction)
	if (i2c_write_single(LCD_ADDR, INSTRUCT, 0b00111001)) {
		puts("error occurred");
	}
	delay_us(20);

	// Internal OSC frequency adjust: 0b0001/BS/F2/F1/F0
	// BS=0, F2F1F0=100
	if (i2c_write_single(LCD_ADDR, INSTRUCT, 0b00010100)) {
		puts("error occurred");
	}
	delay_us(20);

	// Contrast set: 0b0111/C3/C2/C1/C0
	// C3C2C1C0=0011
	if (i2c_write_single(LCD_ADDR, INSTRUCT, 0b01110011)) {
		puts("error occurred");
	}
	delay_us(20);

	// Power/ICON/Contrast control: 0b0101/Ion/Bon/C5/C4
	// Ion=0, Bon=1, C5C4=10
	if (i2c_write_single(LCD_ADDR, INSTRUCT, 0b01010110)) {
		puts("error occurred");
	}
	delay_us(20);

	// Follower Control: 0b0110/Fon/Rab2/Rab1/Rab0
	// Fon=1, Rab2Rab1Rab0=100
	if (i2c_write_single(LCD_ADDR, INSTRUCT, 0b01101100)) {
		puts("error occurred");
	}
	delay_ms(210);

	// Function Set: 0b0011/N/DH/0/IS
	// N=1, DH=0, IS=0
	if (i2c_write_single(LCD_ADDR, INSTRUCT, 0b00111000)) {
		puts("error occurred");
	}
	delay_us(20);
	
	// Clear Display: 0b00000001
	if (i2c_write_single(LCD_ADDR, INSTRUCT, 0x01)) {
		puts("error occurred");
	}
	delay_us(800);
	
	// Display ON: 0b00001/D/C/B
	// D=1(Display), C=1(Cursor), B=0(Blink)
	if (i2c_write_single(LCD_ADDR, INSTRUCT, 0b00001110)) {
		puts("error occurred");
	}
	delay_us(20);
	
	cursor_init();
}

static void set_contrast(uint8_t val)
{
	uint8_t high = (val & 0x30) >> 4;
	uint8_t low  = val & 0x0F;

	set_table(EXTENSION);
	
	// Contrast set: 0b0111/C3/C2/C1/C0
	// C3C2C1C0=0011
	if (i2c_write_single(LCD_ADDR, INSTRUCT, 0x70 | low)) {
		puts("error occurred");
	}
	delay_us(20);

	// Power/ICON/Contrast control: 0b0101/Ion/Bon/C5/C4
	// Ion=0, Bon=1, C5C4=10
	// 0x54 | c5c4
	if (i2c_write_single(LCD_ADDR, INSTRUCT, 0x54 | high)) {
		puts("error occurred");
	}
	delay_us(20);
	
	set_table(NORMAL);
}

void contrast_setting()
{
	cursor_init();
	// Display ON: 0b00001/D/C/B
	// D=1(Display), C=0(Cursor), B=0(Blink)
	if (i2c_write_single(LCD_ADDR, INSTRUCT, 0b00001100)) {
		puts("error occurred");
	}
	delay_us(20);

	// Setting interface
	char contrast[16] = "Contrast = ";
	if (i2c_write_multi(LCD_ADDR, DATAWRITE, (uint8_t *)contrast, strlen(contrast))) {
		puts("error occurred");
	}
	delay_us(20);
	// Move to 2nd line head
	if (i2c_write_single(LCD_ADDR, INSTRUCT, 0xC0)) {
		puts("error occurred");
	}
	delay_us(20);
	char howto[16] = "Adjust by [] key";
	if (i2c_write_multi(LCD_ADDR, DATAWRITE, (uint8_t *)howto, strlen(howto))) {
		puts("error occurred");
	}
	delay_us(20);
	if (i2c_write_single(LCD_ADDR, INSTRUCT, 0x80 | strlen(contrast))) {
		puts("error occurred");
	}
	delay_us(20);
	
	uint8_t conVal = 0x23;
	char dispVal[10];
	
	int loopflg = 1;
	while(loopflg) {
		sprintf(dispVal, "%d", conVal);
		if (i2c_write_multi(LCD_ADDR, DATAWRITE, (uint8_t *)dispVal, strlen(dispVal))) {
			puts("error occurred");
		}
		delay_us(20);
		for (int i=0; i<strlen(dispVal); i++) {
			if (i2c_write_single(LCD_ADDR, INSTRUCT, 0x10)) {
				puts("error occurred");
			}
			delay_us(20);
		}

		set_contrast(conVal);
		char input = getchar();
		if (input == '[' && conVal < 0x3F) {
			beep(UP);
			conVal++;
		} else if (input == ']' && conVal > 0x00) {
			conVal--;
			beep(DOWN);
		} else if (input == '\n') {
			beep(CONF);
			// clear display
			if (i2c_write_single(LCD_ADDR, INSTRUCT, 0x01)) {
				puts("error occurred");
			}
			delay_us(800);
			// Display ON: 0b00001/D/C/B
			// D=1(Display), C=1(Cursor), B=0(Blink)
			if (i2c_write_single(LCD_ADDR, INSTRUCT, 0b00001110)) {
				puts("error occurred");
			}
			delay_us(20);
			loopflg = 0;
		} else if (input == 0x1B) {
			beep(EXIT);
			// clear display
			if (i2c_write_single(LCD_ADDR, INSTRUCT, 0x01)) {
				puts("error occurred");
			}
			delay_us(800);
			// Display ON: 0b00001/D/C/B
			// D=1(Display), C=1(Cursor), B=0(Blink)
			if (i2c_write_single(LCD_ADDR, INSTRUCT, 0b00001110)) {
				puts("error occurred");
			}
			delay_us(20);
			loopflg = 0;
		}
	}
}

void height_setting()
{
	cursor_init();
	// Display ON: 0b00001/D/C/B
	// D=1(Display), C=0(Cursor), B=0(Blink)
	if (i2c_write_single(LCD_ADDR, INSTRUCT, 0b00001100)) {
		puts("error occurred");
	}
	delay_us(20);
	
	// Setting interface
	char height[16] = "D-height = ";
	if (i2c_write_multi(LCD_ADDR, DATAWRITE, (uint8_t *)height, strlen(height))) {
		puts("error occurred");
	}
	delay_us(20);
	// Move to 2nd line head
	if (i2c_write_single(LCD_ADDR, INSTRUCT, 0xC0)) {
		puts("error occurred");
	}
	delay_us(20);
	char howto[16] = "Select by [] key";
	if (i2c_write_multi(LCD_ADDR, DATAWRITE, (uint8_t *)howto, strlen(howto))) {
		puts("error occurred");
	}
	delay_us(20);
	if (i2c_write_single(LCD_ADDR, INSTRUCT, 0x80 | strlen(height))) {
		puts("error occurred");
	}
	delay_us(20);
	
	char sw[10];
	int loopflg = 1;
	while (loopflg) {
		double_height ? strcpy(sw, "ON ") : strcpy(sw, "OFF");
		if (i2c_write_multi(LCD_ADDR, DATAWRITE, (uint8_t *)sw, strlen(sw))) {
			puts("error occurred");
		}
		delay_us(20);
		for (int i=0; i<strlen(sw); i++) {
			if (i2c_write_single(LCD_ADDR, INSTRUCT, 0x10)) {
				puts("error occurred");
			}
			delay_us(20);
		}
		char input = getchar();
		if (input == '[' || input == ']') {
			double_height = double_height ? 0 : 1;
			beep(UP);
		} else if (input == '\n') {
			beep(CONF);
			if (i2c_write_single(LCD_ADDR, INSTRUCT, 0x01)) {
				puts("error occurred");
			}
			delay_us(800);
			// Display ON: 0b00001/D/C/B
			// D=1(Display), C=1(Cursor), B=0(Blink)
			if (i2c_write_single(LCD_ADDR, INSTRUCT, 0b00001110)) {
				puts("error occurred");
			}
			delay_us(20);
			loopflg = 0;
		} else if (input == 0x1B) {
			beep(EXIT);
			// clear display
			if (i2c_write_single(LCD_ADDR, INSTRUCT, 0x01)) {
				puts("error occurred");
			}
			delay_us(800);
			// Display ON: 0b00001/D/C/B
			// D=1(Display), C=1(Cursor), B=0(Blink)
			if (i2c_write_single(LCD_ADDR, INSTRUCT, 0b00001110)) {
				puts("error occurred");
			}
			delay_us(20);
			timer_stop(&TIMER_0);
			loopflg = 0;
		}
	}
	
	// Function Set: 0b001/DL/N/DH/0/IS
	// DL=1(8bit), N=!double_height, DH=double_height, IS=0(normal instruction)
	uint8_t param = double_height ? 0b00110100 : 0b00111000;
	if (i2c_write_single(LCD_ADDR, INSTRUCT, param)) {
		puts("error occurred");
	}
	delay_us(20);
}

static void delete_char()
{
	// Write space code (0x20)
	if (i2c_write_single(LCD_ADDR, DATAWRITE, 0x20)) {
		puts("error occurred");
	}
	delay_us(20);
	// Fix cursor position
	if (i2c_write_single(LCD_ADDR, INSTRUCT, 0b00010000)) {
		puts("error occurred");
	}
	delay_us(20);
}

void mirror_input()
{
	// Display ON: 0b00001/D/C/B
	// D=1(Display), C=1(Cursor), B=0(Blink)
	if (i2c_write_single(LCD_ADDR, INSTRUCT, 0b00001110)) {
		puts("error occurred");
	}
	delay_us(20);
	cursor_init();
	
	int cursor_pos = 0;
	int loopflg = 1;
	while (loopflg) {
		if (cursor_pos == MAX_OF_LINE) {
			// Set DDRAM Address: 0b1/AC6/AC5/AC4/AC3/AC2/AC1/AC0
			// 2nd line is from 40H to 67H
			int dir = getHeight() ? 0x80 : 0xC0;
			if (i2c_write_single(LCD_ADDR, INSTRUCT, dir)) {
				puts("error occurred");
			}
			delay_us(20);
			
			if (getHeight()) {
				cursor_pos = 0;
			}
		}
		if (cursor_pos == MAX_OF_LINE*2) {
			// 1st line is from 00H to 27H
			if (i2c_write_single(LCD_ADDR, INSTRUCT, 0x80)) {
				puts("error occurred");
			}
			cursor_pos = 0;
		}
		
		char input = getchar();
		beep(CONF);
		
		if (input == '\b') {
			if (cursor_pos == 0) {
				// Move to 2nd line end
				int dir = getHeight() ? 0x8F : 0xCF;
				if (i2c_write_single(LCD_ADDR, INSTRUCT, dir)) {
					puts("error occurred");
				}
				delay_us(20);
				cursor_pos = getHeight() ? MAX_OF_LINE - 1 : MAX_OF_LINE*2 - 1;
			} else if (cursor_pos == 16) {
				// Move to 1st line end
				if (i2c_write_single(LCD_ADDR, INSTRUCT, 0x8F)) {
					puts("error occurred");
				}
				delay_us(20);
				cursor_pos = MAX_OF_LINE - 1;
			} else {
				// Shift cursor to the left
				if (i2c_write_single(LCD_ADDR, INSTRUCT, 0b00010000)) {
					puts("error occurred");
				}
				delay_us(20);
				cursor_pos--;
			}
			delete_char();
		// Limit display characters from stdin
		} else if ('!' < input && input < '}' || input == 0x20) {
			if (i2c_write_single(LCD_ADDR, DATAWRITE, (uint8_t)input)) {
				puts("error");
			}
			delay_us(20);
			cursor_pos++;
		// Exit by ESC
		} else if (input == 0x1B) {
			beep(EXIT);
			// Clear Display: 0b00000001
			if (i2c_write_single(LCD_ADDR, INSTRUCT, 0x01)) {
				puts("error occurred");
			}
			delay_us(800);
			loopflg = 0;
		}
	}
}
