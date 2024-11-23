#ifndef INC_HX711_H_
#define INC_HX711_H_

#include "abstract.h"
#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "stdbool.h"

typedef struct HX711_settings
{
	GPIO_TypeDef*		DT_Port;
	uint16_t			DT_Pin;
	GPIO_TypeDef*		SCK_Port;
	uint16_t			SCK_Pin;
	uint32_t			tare;
	float 				knownOriginal;
	float				knownHX711;
	int32_t  			samples;
} HX711;

HX711 HX711_Create();
void microDelay(uint16_t delay);
void HX711_Init(HX711* _HX711);
int32_t HX711_Calculate(HX711* _HX711);
void HX711_GetData(HX711* _HX711, float *peso);

#endif
