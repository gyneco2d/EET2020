/*
 * lcd_lib.h
 *
 * Created: 2020/06/29 2:04:32
 *  Author: gyneco2d
 */ 

#ifndef LCD_LIB_H_
#define LCD_LIB_H_

// 7bit slave addr: 0b0111110 = 0x3E
#define LCD_ADDR 0x3E

// Control byte: Co/RS/000000
// Co 0: last control byte
// RS 0: Instruction
//    1: Data write
#define INSTRUCT  0x00
#define DATAWRITE 0x40

// Max number of characters per line that can be displayed
#define MAX_OF_LINE 16

#define ARR_LEN(arr) sizeof(arr) / sizeof(arr[0])

typedef enum {
	NORMAL,
	EXTENSION
} instruction_table;

void set_table(instruction_table table);
int getHeight();
void cursor_init();
void lcd_init();
void contrast_setting();
void height_setting();
void mirror_input();

#endif /* LCD_LIB_H_ */
