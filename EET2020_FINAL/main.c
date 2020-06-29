#include <atmel_start.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/i2c_ext_lib.h"
#include "lib/i2c_lib.h"
#include "lib/lcd_lib.h"
#include "lib/sonner.h"
#include "lib/beep.h"

void sonner()
{
	cursor_init();
	sonner_init();
	timer_start(&TIMER_0);
	delay_ms(500);
	
	// Display ON: 0b00001/D/C/B
	// D=1(Display), C=0(Cursor), B=0(Blink)
	if (i2c_write_single(LCD_ADDR, INSTRUCT, 0b00001100)) {
		puts("error occurred");
	}
	delay_us(20);

	// Setting interface
	char caption[16] = "dist. ";
	if (i2c_write_multi(LCD_ADDR, DATAWRITE, (uint8_t *)caption, strlen(caption))) {
		puts("error occurred");
	}
	delay_us(20);
	// Move to 2nd line head
	if (i2c_write_single(LCD_ADDR, INSTRUCT, 0xC0)) {
		puts("error occurred");
	}
	delay_us(20);
	char howto[16] = "Enter to update";
	if (i2c_write_multi(LCD_ADDR, DATAWRITE, (uint8_t *)howto, strlen(howto))) {
		puts("error occurred");
	}
	delay_us(20);
	if (i2c_write_single(LCD_ADDR, INSTRUCT, 0x80 | strlen(caption))) {
		puts("error occurred");
	}
	delay_us(20);
	
	char centi[10] = {'\0'};

	int loopflg = 1;
	while (loopflg) {
		// Display distance
		int dist = getDistance();
		sprintf(centi, "%3d.%dcm", dist/10, dist%10);
		if (i2c_write_multi(LCD_ADDR, DATAWRITE, (uint8_t *)centi, strlen(centi))) {
			puts("error occurred");
		}
		delay_us(20);
		// Fix cursor position
		for (int i=0; i<strlen(centi); i++) {
			if (i2c_write_single(LCD_ADDR, INSTRUCT, 0x10)) {
				puts("error occurred");
			}
			delay_us(20);
		}
		
		char input;
		while (1) {
			input = getchar();
			if (input == '\n' || input == 0x1B) {
				break;
			}
		}
		
		if (input == '\n') {
			beep(CONF);
			// Clear current distance
			for (int i=0; i<strlen(centi); i++) {
				if (i2c_write_single(LCD_ADDR, DATAWRITE, 0x20)) {
					puts("error occurred");
				}
				delay_us(20);
			}
			// Fix cursor position
			for (int i=0; i<strlen(centi); i++) {
				if (i2c_write_single(LCD_ADDR, INSTRUCT, 0x10)) {
					puts("error occurred");
				}
				delay_us(20);
			}
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
}

void clean_display()
{
	if (i2c_write_single(LCD_ADDR, INSTRUCT, 0x01)) {
		puts("error occurred");
	}
	delay_us(800);
}

void disp_menu()
{
	// Display ON: 0b00001/D/C/B
	// D=1(Display), C=0(Cursor), B=0(Blink)
	if (i2c_write_single(LCD_ADDR, INSTRUCT, 0b00001100)) {
		puts("error occurred");
	}
	delay_us(20);
	clean_display();

	// Menu
	char menu_line1[40] = "1.Mirror 2.Contrast ";
	if (i2c_write_multi(LCD_ADDR, DATAWRITE, (uint8_t *)menu_line1, strlen(menu_line1))) {
		puts("error occurred");
	}
	delay_us(20);
	if (!getHeight()) {
		// Move to 2nd line head
		if (i2c_write_single(LCD_ADDR, INSTRUCT, 0xC0)) {
			puts("error occurred");
		}
		delay_us(20);
	}
	char menu_line2[40] = "3.Height 4.Sonner ";
	if (i2c_write_multi(LCD_ADDR, DATAWRITE, (uint8_t *)menu_line2, strlen(menu_line2))) {
		puts("error occurred");
	}
	delay_us(20);
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	i2c_init();
	lcd_init();
	puts("Start");

	int loopflg = 1;
	while (loopflg) {
		disp_menu();
		switch (getchar()) {
			case '1':
				beep(CONF);
				clean_display();
				mirror_input();
				break;
			case '2':
				beep(CONF);
				clean_display();
				contrast_setting();
				break;
			case '3':
				beep(CONF);
				clean_display();
				height_setting();
				break;
			case '4':
				beep(CONF);
				clean_display();
				sonner();
				break;
			case 'h':
				// Shift display to the left
				if (i2c_write_single(LCD_ADDR, INSTRUCT, 0b00011100)) {
					puts("error occurred");
				}
				delay_us(20);
				break;
			case 'l':
				// Shift display to the right
				if (i2c_write_single(LCD_ADDR, INSTRUCT, 0b00011000)) {
					puts("error occurred");
				}
				delay_us(20);
				break;
			default:
				break;
		}
	}
}
