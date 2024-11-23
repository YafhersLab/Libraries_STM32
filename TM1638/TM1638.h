#ifndef INC_TM1638_H_
#define INC_TM1638_H_

// Librerias
#include "main.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

// Definiciones
#define LSBFIRST	0
#define MSBFIRST 	1
#define DIO_INPUT 0
#define DIO_OUTPUT 1
#define DISPLAY_0	0x3F
#define DISPLAY_1 0x06
#define DISPLAY_2 0x5B
#define DISPLAY_3 0x4F
#define DISPLAY_4 0x66
#define DISPLAY_5 0x6D
#define DISPLAY_6 0x7D
#define DISPLAY_7 0x07
#define DISPLAY_8 0x7F
#define DISPLAY_9 0x67
#define DISPLAY_A 0x77
#define DISPLAY_E 0x79
#define DISPLAY_H 0x76
#define DISPLAY_I 0x06
#define DISPLAY_L 0x38
#define DISPLAY_O 0x3F
#define DISPLAY_S 0x6D
#define DISPLAY_X 0x76

// Estructuras
struct TM1638_Handler{
	GPIO_TypeDef* STB_Port;
	GPIO_TypeDef* CLK_Port;
	GPIO_TypeDef* DIO_Port;
	uint16_t STB_Pin;
	uint16_t CLK_Pin;
	uint16_t DIO_Pin;
};

// Funciones
void TM1638_Init(struct TM1638_Handler *TM1638);
void TM1638_writeLed(uint8_t led, uint8_t value);
void TM1638_writeDisplay(uint8_t display, uint8_t command);
uint8_t TM1638_readButton(uint8_t btn);
void TM1638_modeDIO(uint8_t mode);
void shiftOut(uint8_t bitOrder, uint8_t value);
uint8_t shiftIn(uint8_t bitOrder);

#endif