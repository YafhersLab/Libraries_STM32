#include "LCD.h"

struct LCD_Handler _LCD = {0};

void LCD_Init(struct LCD_Handler *LCD)
{
	// Asigno los pines
	_LCD.D4_PIN = LCD -> D4_PIN;
	_LCD.D5_PIN = LCD -> D5_PIN;
	_LCD.D6_PIN = LCD -> D6_PIN;
	_LCD.D7_PIN = LCD -> D7_PIN;
	_LCD.RS_PIN = LCD -> RS_PIN;
	_LCD.E_PIN = LCD -> E_PIN;

	// Asigno los puertos
	_LCD.D4_PORT = LCD -> D4_PORT;
	_LCD.D5_PORT = LCD -> D5_PORT;
	_LCD.D6_PORT = LCD -> D6_PORT;
	_LCD.D7_PORT = LCD -> D7_PORT;
	_LCD.RS_PORT = LCD -> RS_PORT;
	_LCD.E_PORT = LCD -> E_PORT;

	// Envio los comandos de inicio
	HAL_GPIO_WritePin(_LCD.RS_PORT, _LCD.RS_PIN, 0);
	HAL_Delay(15);	LCD_Command(0x03);
	HAL_Delay(4);	LCD_Command(0x03);
	HAL_Delay(1);	LCD_Command(0x03);
	HAL_Delay(1);	LCD_Command(0x02);
	LCD_Command(0x02);
	LCD_Command(0x08); //16x2
	LCD_Command(0x00);
	LCD_Command(0x08);
	LCD_Command(0x00);
	LCD_Command(0x01);
	LCD_Command(0x00);
	LCD_Command(0x0D); //No Cursor: 0XC
}

void LCD_Command(uint8_t cmd)
{
	// Prendo y apago el pin E
	HAL_GPIO_WritePin(_LCD.E_PORT, _LCD.E_PIN, 0);	HAL_Delay(2);
	HAL_GPIO_WritePin(_LCD.E_PORT, _LCD.E_PIN, 1);	HAL_Delay(2);

	// Escribo el primer bit (0001)
	if(cmd & 0x01)	HAL_GPIO_WritePin(_LCD.D4_PORT, _LCD.D4_PIN, 1);
	else	HAL_GPIO_WritePin(_LCD.D4_PORT, _LCD.D4_PIN, 0);
	
	// Escribo el segundo bit (0010)
	if(cmd & 0x02)	HAL_GPIO_WritePin(_LCD.D5_PORT, _LCD.D5_PIN, 1);
	else	HAL_GPIO_WritePin(_LCD.D5_PORT, _LCD.D5_PIN, 0);

	// Escribo el tercer bit (0100)
	if(cmd & 0x04)	HAL_GPIO_WritePin(_LCD.D6_PORT, _LCD.D6_PIN, 1);
	else	HAL_GPIO_WritePin(_LCD.D6_PORT, _LCD.D6_PIN, 0);

	// Escribo el segundo bit (0010)
	if(cmd & 0x08)	HAL_GPIO_WritePin(_LCD.D7_PORT, _LCD.D7_PIN, 1);
	else	HAL_GPIO_WritePin(_LCD.D7_PORT, _LCD.D7_PIN, 0);

	// Apagamos el pin E
	HAL_Delay(2);
	HAL_GPIO_WritePin(_LCD.E_PORT, _LCD.E_PIN, 0);
}

void LCD_PrintChar(char data)
{
	// Activo el pin RS
	HAL_GPIO_WritePin(_LCD.RS_PORT, _LCD.RS_PIN, 1);

	// Envio el caracter en 2 partes
	LCD_Command(data >> 4);
	LCD_Command(data);
}

void LCD_PrintString(char *str)
{
	while(*str)
	{
		LCD_PrintChar(*str++);
	}
}

void LCD_GotoXY(int x, int y)
{
	// Apagamos el pin RS
	HAL_GPIO_WritePin(_LCD.RS_PORT, _LCD.RS_PIN, 0);

	// Nos posicionamos en la fila segun la columna
	switch(y)
	{
		case 1:
			LCD_Command(0x80 + x);
			LCD_Command((0x80 + x) >> 4);
			break;
		case 2:
			LCD_Command(0xC0 + x);
			LCD_Command((0xC0 + x) >> 4);
			break;
		case 3:
			LCD_Command(0x80 + 20 + x);
			LCD_Command((0x80 + 20 + x) >> 4);
			break;
		case 4:
			LCD_Command(0xC0 + 20 + x);
			LCD_Command((0xC0 + 20 + x) >> 4);
			break;
	}
}

void Lcd_Clear(void)
{
	HAL_GPIO_WritePin(_LCD.RS_PORT, _LCD.RS_PIN, 0);
	LCD_Command(0x01);
	HAL_Delay(2);
}

void Lcd_Blink(void)
{
	HAL_GPIO_WritePin(_LCD.RS_PORT, _LCD.RS_PIN, 0);
	LCD_Command(0x0F);
}

void Lcd_NoBlink(void)
{
	HAL_GPIO_WritePin(_LCD.RS_PORT, _LCD.RS_PIN, 0);
	LCD_Command(0x0C);
}
