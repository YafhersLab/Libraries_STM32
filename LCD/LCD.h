#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "main.h"

struct LCD_Handler{
	GPIO_TypeDef* D4_PORT;
	GPIO_TypeDef* D5_PORT;
	GPIO_TypeDef* D6_PORT;
	GPIO_TypeDef* D7_PORT;
	GPIO_TypeDef* E_PORT;
	GPIO_TypeDef* RS_PORT;
	uint16_t D4_PIN;
	uint16_t D5_PIN;
	uint16_t D6_PIN;
	uint16_t D7_PIN;
	uint16_t RS_PIN;
	uint16_t E_PIN;
};

void LCD_Command(uint8_t cmd);
void LCD_Init(struct LCD_Handler *LCD);
void LCD_PrintChar(char data);
void LCD_PrintString(char *str);
void LCD_GotoXY(int row, int col);
void LCD_Clear(void);
void LCD_Blink(void);
void LCD_NoBlink(void);
//void Lcd_CGRAM_CreateChar(unsigned char pos, const char*msg);
//void Lcd_CGRAM_WriteChar(char pos);

#endif
