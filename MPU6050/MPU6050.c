// Librerias
#include "MPU6050.h"

// Definiciones
#define GIRO_CAL 	(float) 131.0
#define ACCEL_CAL 	(float) 16384.0
#define TEMP_CAL 	(float) 376.53
#define RAD_A_DEG 	(float) 57.295779

// Registros MPU6050
#define PWR_MGMT_1 		0x6B
#define SMPLRT_DIV		0X19
#define GYRO_CONFIG		0x1B
#define ACCEL_CONFIG	0x1C
#define CONFIG			0x1A
#define FIFO_COUNTH  	0x72
#define FIFO_R_W     	0x74
#define USER_CTRL    	0x6A
#define INT_ENABLE   	0x38
#define FIFO_EN      	0x23

// Variables
uint8_t _MPU6050_TX[2];
uint8_t _MPU6050_RX[15];
I2C_HandleTypeDef _i2cMPU;

struct MPU6050_Handler _MPU_Values = {};
struct MPU6050_Quaternion _MPU_Quat = {};

// Desarrollo de Funciones
void MPU6050_Init(I2C_HandleTypeDef *i2cMPU)
{
	_i2cMPU = *i2cMPU;

	MPU6050_Write(PWR_MGMT_1, 0x00); // PWR_MGMT_1 -> Usar reloj 8KHz interno
	MPU6050_Write(SMPLRT_DIV, 0x07); // SMPLRT_DIV -> Muestreo de 1KHz
	MPU6050_Write(GYRO_CONFIG, 0x00); // GYRO_CONFIG -> Giroscopio con rango ± 250 °/s
	MPU6050_Write(ACCEL_CONFIG, 0x00); // ACCEL_CONFIG -> Acelerometro con rango ± 2g
	MPU6050_Write(CONFIG, 0x03); // CONFIG -> Filtro de paso bajo digital 0x03
	MPU6050_Write(USER_CTRL, 0x40); // USER_CTRL -> Resetear sensores
	MPU6050_Write(FIFO_EN, 0xF8); // FIFO_EN -> Habilita FIFO en SLV y ACCEL
	MPU6050_Write(INT_ENABLE, 0x01); // INT_ENABLE -> Habilitar interrupciones
}

void MPU6050_Write(uint8_t reg, uint8_t value)
{
    uint8_t data[2] = {reg, value};
    HAL_I2C_Master_Transmit(&_i2cMPU, (uint16_t)MPU6050_ADDRESS, data, 2, 100);
}

uint8_t MPU6050_Read(void)
{
	_MPU6050_TX[0] = 0x3B; // Dirección Inicial: ACCEL_XOUT_H (0x3B)
	HAL_I2C_Master_Transmit(&_i2cMPU, (uint16_t)MPU6050_ADDRESS, _MPU6050_TX, 1, 100);

	// Recibimos 14 bytes desde la dirección 0x3B (6 de giroscopio, 6 de acelerometro y 2 de temperatura)
	HAL_StatusTypeDef estado_MPU = HAL_I2C_Master_Receive(&_i2cMPU, (uint16_t)MPU6050_ADDRESS, _MPU6050_RX, 14, 100);
	_MPU_Values.MPU_Ax = (float)( ((int16_t)(_MPU6050_RX[0]<<8 | _MPU6050_RX[1])) / ACCEL_CAL);
	_MPU_Values.MPU_Ay = (float)( ((int16_t)(_MPU6050_RX[2]<<8 | _MPU6050_RX[3])) / ACCEL_CAL);
	_MPU_Values.MPU_Az =(float)( ((int16_t)(_MPU6050_RX[4]<<8 | _MPU6050_RX[5])) / ACCEL_CAL);
	_MPU_Values.MPU_Gx = (float)( ((int16_t)(_MPU6050_RX[10]<<8 | _MPU6050_RX[11])) / GIRO_CAL);
	_MPU_Values.MPU_Gy = (float)( ((int16_t)(_MPU6050_RX[12]<<8 | _MPU6050_RX[13])) / GIRO_CAL);
	_MPU_Values.MPU_Gz = (float)( ((int16_t)(_MPU6050_RX[8]<<8 | _MPU6050_RX[9])) / GIRO_CAL);
	_MPU_Values.MPU_Temp = (float)( ((int16_t)(_MPU6050_RX[6]<<8 | _MPU6050_RX[7])) / TEMP_CAL);

	return (estado_MPU == HAL_OK) ? 1 : 0;
}

uint8_t MPU6050_ReadFIFO(void)
{
	// El FIFO devuelve 12 bytes para quaterniones
    uint8_t buffer[12];
    uint16_t fifo_count = 0;

    // Leer el conteo de datos en el FIFO
    _MPU6050_TX[0] = FIFO_COUNTH;
    HAL_I2C_Master_Transmit(&_i2cMPU, (uint16_t)MPU6050_ADDRESS, _MPU6050_TX, 1, 100);
    HAL_StatusTypeDef estado_MPU = HAL_I2C_Master_Receive(&_i2cMPU, (uint16_t)MPU6050_ADDRESS, _MPU6050_RX, 2, 100);
    fifo_count = (_MPU6050_RX[0] << 8) | _MPU6050_RX[1];

    // Si no hay suficientes datos, retornamos error
    if (fifo_count < 12) return 0;

    // Leer los 12 bytes del FIFO
    _MPU6050_TX[0] = FIFO_R_W;
    HAL_I2C_Master_Transmit(&_i2cMPU, (uint16_t)MPU6050_ADDRESS, _MPU6050_TX, 1, 100);
    HAL_I2C_Master_Receive(&_i2cMPU, (uint16_t)MPU6050_ADDRESS, buffer, 12, 100);

    // Convertir los datos en quaterniones
    _MPU_Quat.q0 = ((int16_t)(buffer[0] << 8 | buffer[1])) / 16384.0f;
    _MPU_Quat.q1 = ((int16_t)(buffer[4] << 8 | buffer[5])) / 16384.0f;
    _MPU_Quat.q2 = ((int16_t)(buffer[8] << 8 | buffer[9])) / 16384.0f;
    _MPU_Quat.q3 = ((int16_t)(buffer[6] << 8 | buffer[7])) / 16384.0f;

    // Obtengo Yaw, Pitch y Roll
    _MPU_Values.MPU_Yaw = atan2(2.0f * (_MPU_Quat.q0 * _MPU_Quat.q3 + _MPU_Quat.q1 * _MPU_Quat.q2), 1.0f - 2.0f * (_MPU_Quat.q2 * _MPU_Quat.q2 + _MPU_Quat.q3 * _MPU_Quat.q3)) * RAD_A_DEG;
    _MPU_Values.MPU_Pitch = asin(2.0f * (_MPU_Quat.q0 * _MPU_Quat.q2 - _MPU_Quat.q3 * _MPU_Quat.q1)) * RAD_A_DEG;
    _MPU_Values.MPU_Roll = atan2(2.0f * (_MPU_Quat.q0 * _MPU_Quat.q1 + _MPU_Quat.q2 * _MPU_Quat.q3), 1.0f - 2.0f * (_MPU_Quat.q1 * _MPU_Quat.q1 + _MPU_Quat.q2 * _MPU_Quat.q2)) * RAD_A_DEG;

    return (estado_MPU == HAL_OK) ? 1 : 0;
}

float MPU6050_getAx(void)
{
	return _MPU_Values.MPU_Ax;
}

float MPU6050_getAy(void)
{
	return _MPU_Values.MPU_Ay;
}

float MPU6050_getAz(void)
{
	return _MPU_Values.MPU_Az;
}

float MPU6050_getGx(void)
{
	return _MPU_Values.MPU_Gx;
}

float MPU6050_getGy(void)
{
	return _MPU_Values.MPU_Gy;
}

float MPU6050_getGz(void)
{
	return _MPU_Values.MPU_Gz;
}

float MPU6050_getTemperature(void)
{
	return _MPU_Values.MPU_Temp;
}

float MPU6050_getYaw(void)
{
	return _MPU_Values.MPU_Yaw;
}

float MPU6050_getPitch(void)
{
	return _MPU_Values.MPU_Pitch;
}

float MPU6050_getRoll(void) {
	return _MPU_Values.MPU_Roll;
}
