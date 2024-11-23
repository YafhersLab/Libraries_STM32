#include "HX711.h"
#include "portmacro.h"
#include <rom/ets_sys.h>

//extern TIM_HandleTypeDef htim2;

HX711 HX711_Create()
{
	HX711 new_HX711;

	new_HX711.knownHX711 = 1;
	new_HX711.knownOriginal = 1;
	new_HX711.tare = 0;
	new_HX711.samples = 10;

	return new_HX711;
}

void microDelay(uint16_t delay)
{
	WAIT_US(NULL, delay);
}


void HX711_Init(HX711* _HX711)
{
	SET_PIN(_HX711 -> SCK_Port, _HX711 -> SCK_Pin);
	WAIT_MS(10);
	RESET_PIN(_HX711 -> SCK_Port, _HX711 -> SCK_Pin);
	WAIT_MS(10);
}

int32_t HX711_Calculate(HX711* _HX711)
{
	uint32_t data = 0;
	uint32_t startTime = GET_TIME;

	while(READ_PIN(_HX711 -> DT_Port, _HX711 -> DT_Pin) == 1)
	{
		if(GET_TIME - startTime > 200)	return 0;
	}

	for(int8_t len = 0; len < 24 ; len++)
	{
		SET_PIN(_HX711 -> SCK_Port, _HX711 -> SCK_Pin);
		microDelay(1);
		data = data << 1;
		RESET_PIN(_HX711 -> SCK_Port, _HX711 -> SCK_Pin);
		microDelay(1);

		if(READ_PIN(_HX711 -> DT_Port, _HX711 -> DT_Pin) == 1)
		{
			data ++;
		}
	}

	data = data ^ 0x800000;

	SET_PIN(_HX711 -> SCK_Port, _HX711 -> SCK_Pin);
	microDelay(1);
	RESET_PIN(_HX711 -> SCK_Port, _HX711 -> SCK_Pin);
  	microDelay(1);

  	return data;

}

void HX711_GetData(HX711* _HX711, float *peso)
{
	int32_t  total = 0;

	for(uint16_t i = 0 ; i < _HX711 -> samples ; i++)
	{
		total += HX711_Calculate(_HX711);
	}

	int32_t average = (int32_t) (total / _HX711 -> samples);
	float coefficient = _HX711 -> knownOriginal / _HX711 -> knownHX711;
	int milligram = (int) (average - _HX711 -> tare) * coefficient;

	*peso = (float) milligram / 1000.0;
}
