// Librerias
#include "TM1638.h"

// Estructura Interna
struct TM1638_Handler _TM1638 = {};	
	
// Iniciación del TM1638
void TM1638_Init(struct TM1638_Handler *TM1638)
{
	// Asignación de Pines
	_TM1638.STB_Port = TM1638 -> STB_Port;
	_TM1638.CLK_Port = TM1638 -> CLK_Port;
	_TM1638.DIO_Port = TM1638 -> DIO_Port;
	_TM1638.STB_Pin = TM1638 -> STB_Pin;
	_TM1638.CLK_Pin = TM1638 -> CLK_Pin;
	_TM1638.DIO_Pin = TM1638 -> DIO_Pin;
	
	// Asignación del brillo del TM1638
	HAL_GPIO_WritePin(_TM1638.STB_Port, _TM1638.STB_Pin, 0);
  shiftOut(LSBFIRST, 0x8F); //F - 0 brillo
	HAL_GPIO_WritePin(_TM1638.STB_Port, _TM1638.STB_Pin, 1);
	
	// Usamos el modo autoincremento de dirección para limpiar los leds y los displays
	HAL_GPIO_WritePin(_TM1638.STB_Port, _TM1638.STB_Pin, 0);
  shiftOut(LSBFIRST, 0x40); // 0x40 -> autoincremento de direccion
	HAL_GPIO_WritePin(_TM1638.STB_Port, _TM1638.STB_Pin, 1);
	
	// Limpiamos los leds y los displays
  HAL_GPIO_WritePin(_TM1638.STB_Port, _TM1638.STB_Pin, 0);
  shiftOut(LSBFIRST, 0xC0);    // Direccion C0 a CF
  for(int i = 0; i < 16; i++) shiftOut(LSBFIRST, 0);
  HAL_GPIO_WritePin(_TM1638.STB_Port, _TM1638.STB_Pin, 1);
	
	// Usamos el modo de dirección única por defecto
	HAL_GPIO_WritePin(_TM1638.STB_Port, _TM1638.STB_Pin, 0);
  shiftOut(LSBFIRST, 0x44); // 0x44 -> direccion unica
	HAL_GPIO_WritePin(_TM1638.STB_Port, _TM1638.STB_Pin, 1);
}

// Funcion para prender o apagar los leds del TM1638
void TM1638_writeLed(uint8_t led, uint8_t value)
{
	HAL_GPIO_WritePin(_TM1638.STB_Port, _TM1638.STB_Pin, 0);
	
	// Elijo el led a prender
	switch(led)
	{
		case 1:
			shiftOut(LSBFIRST, 0xC1); break;
		case 2:
			shiftOut(LSBFIRST, 0xC3);	break;
		case 3:
			shiftOut(LSBFIRST, 0xC5); break;
		case 4:
			shiftOut(LSBFIRST, 0xC7); break;
		case 5:
			shiftOut(LSBFIRST, 0xC9); break;
		case 6:
			shiftOut(LSBFIRST, 0xCB); break;
		case 7:
			shiftOut(LSBFIRST, 0xCD); break;
		case 8:
			shiftOut(LSBFIRST, 0xCF); break;
		default:
			break;
	}
	
	// Envio el estado del led
	shiftOut(LSBFIRST, value);
	HAL_GPIO_WritePin(_TM1638.STB_Port, _TM1638.STB_Pin, 1);
}

// Funcion para prender o apagar los leds del Display de 7 segmentos
void TM1638_writeDisplay(uint8_t display, uint8_t command)
{
	HAL_GPIO_WritePin(_TM1638.STB_Port, _TM1638.STB_Pin, 0);
	
	// Elijo el display a encender
	switch(display)
	{
		case 1:
			shiftOut(LSBFIRST, 0xC0); break;
		case 2:
			shiftOut(LSBFIRST, 0xC2);	break;
		case 3:
			shiftOut(LSBFIRST, 0xC4); break;
		case 4:
			shiftOut(LSBFIRST, 0xC6); break;
		case 5:
			shiftOut(LSBFIRST, 0xC8); break;
		case 6:
			shiftOut(LSBFIRST, 0xCA); break;
		case 7:
			shiftOut(LSBFIRST, 0xCC); break;
		case 8:
			shiftOut(LSBFIRST, 0xCE); break;
		default:
			break;
	}
	
	// Elijo el comando a enviar
	switch(command)
	{
		case 0:
			shiftOut(LSBFIRST, DISPLAY_0); break;
		case 1:
			shiftOut(LSBFIRST, DISPLAY_1); break;
		case 2:
			shiftOut(LSBFIRST, DISPLAY_2);	break;
		case 3:
			shiftOut(LSBFIRST, DISPLAY_3); break;
		case 4:
			shiftOut(LSBFIRST, DISPLAY_4); break;
		case 5:
			shiftOut(LSBFIRST, DISPLAY_5); break;
		case 6:
			shiftOut(LSBFIRST, DISPLAY_6); break;
		case 7:
			shiftOut(LSBFIRST, DISPLAY_7); break;
		case 8:
			shiftOut(LSBFIRST, DISPLAY_8); break;
		case 9:
			shiftOut(LSBFIRST, DISPLAY_9); break;
		default:
			shiftOut(LSBFIRST, command); break;
	}
	
	HAL_GPIO_WritePin(_TM1638.STB_Port, _TM1638.STB_Pin, 1);
}

// Funcion para leer el estado de los botones
uint8_t TM1638_readButton(uint8_t btn)
{	
	uint8_t buttons = 0;
	uint8_t state = 0;
	
	// Envio el comando de lectura de botones
	HAL_GPIO_WritePin(_TM1638.STB_Port, _TM1638.STB_Pin, 0);
  shiftOut(LSBFIRST, 0x42);    // 0x42 -> Leer botones
	TM1638_modeDIO(DIO_INPUT);
	
	// Obtengo los 4 bytes de los botones presionados
	for(uint8_t i = 0; i < 4; i++)
	{
		uint8_t val = shiftIn(LSBFIRST) << i; 
		buttons |= val;
	}
	
	// Lectura del boton ingresado
	switch(btn)
	{
		case 1:
			if(buttons == 0x01) state = 1;
			else state = 0; break;
		case 2:
			if(buttons == 0x02) state = 1;
			else state = 0; break;
		case 3:
			if(buttons == 0x04) state = 1;
			else state = 0; break;
		case 4:
			if(buttons == 0x08) state = 1;
			else state = 0; break;
		case 5:
			if(buttons == 0x10) state = 1;
			else state = 0; break;
		case 6:
			if(buttons == 0x20) state = 1;
			else state = 0; break;
		case 7:
			if(buttons == 0x40) state = 1;
			else state = 0; break;
		case 8:
			if(buttons == 0x80) state = 1;
			else state = 0; break;
	}
	
	// Terminamos la comunicacion
	TM1638_modeDIO(DIO_OUTPUT);
	HAL_GPIO_WritePin(_TM1638.STB_Port, _TM1638.STB_Pin, 1);
	
	if(state == 1) return 1;
	else return 0;
}

// Para establecer el modo del DIO
void TM1638_modeDIO(uint8_t mode)
{
	GPIO_InitTypeDef DIO_GPIO = {};
		
	if(mode == DIO_INPUT)
	{
		DIO_GPIO.Mode = GPIO_MODE_INPUT;
		DIO_GPIO.Pull = GPIO_NOPULL;
		DIO_GPIO.Speed = GPIO_SPEED_FREQ_HIGH;
		DIO_GPIO.Pin = _TM1638.DIO_Pin;
		HAL_GPIO_Init(_TM1638.DIO_Port, &DIO_GPIO);
	}
	else if(mode == DIO_OUTPUT)
	{
		DIO_GPIO.Mode = GPIO_MODE_OUTPUT_PP;
		DIO_GPIO.Pull = GPIO_NOPULL;
		DIO_GPIO.Speed = GPIO_SPEED_FREQ_HIGH;
		DIO_GPIO.Pin = _TM1638.DIO_Pin;
		HAL_GPIO_Init(_TM1638.DIO_Port, &DIO_GPIO);
	}
}

// Funcion para enviar los datos de manera serial
void shiftOut(uint8_t bitOrder, uint8_t value) 
{
	for (uint8_t i = 0; i < 8; i++) 
	{
		// Verificamos por que bit empezar a enviar los datos
		if (bitOrder == LSBFIRST) 		HAL_GPIO_WritePin(_TM1638.DIO_Port, _TM1638.DIO_Pin, (value & (1 << i))? 				1 : 0);
		else if (bitOrder == MSBFIRST) HAL_GPIO_WritePin(_TM1638.DIO_Port, _TM1638.DIO_Pin, (value & (1 << (7 - i)))? 	1 : 0);

		// Generamos el pulso de reloj
		HAL_GPIO_WritePin(_TM1638.CLK_Port, _TM1638.CLK_Pin, 1);		HAL_Delay(1);
		HAL_GPIO_WritePin(_TM1638.CLK_Port, _TM1638.CLK_Pin, 0);		HAL_Delay(1);
	}
}

// Funcion para leer los datos de manera serial
uint8_t shiftIn(uint8_t bitOrder) 
{
	uint8_t value = 0;

	for (uint8_t i = 0; i < 8; i++) 
	{
		// 	Generamos el pulso de reloj
		HAL_GPIO_WritePin(_TM1638.CLK_Port, _TM1638.CLK_Pin, 1);		HAL_Delay(1);

		// Verificamos por que bit empezar a recibir los datos
		if (bitOrder == LSBFIRST)	value |= (HAL_GPIO_ReadPin(_TM1638.DIO_Port, _TM1638.DIO_Pin) << i);
		else if (bitOrder == MSBFIRST) value |= (HAL_GPIO_ReadPin(_TM1638.DIO_Port, _TM1638.DIO_Pin) << (7 - i));

		HAL_GPIO_WritePin(_TM1638.CLK_Port, _TM1638.CLK_Pin, 0);		HAL_Delay(1);
	}

	return value;
}