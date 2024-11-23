#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

// Librerias
#include "i2c.h"
#include <stdint.h>
#include <math.h>

// Definiciones
#define MPU6050_ADDRESS 0x68 << 1

// Estructuras
struct MPU6050_Handler{
	float MPU_Ax;
	float MPU_Ay;
	float MPU_Az;
	float MPU_Gx;
	float MPU_Gy;
	float MPU_Gz;
	float MPU_Temp;
	// Yaw, Pitch, Roll solo disponible con FIFO
	float MPU_Yaw;
	float MPU_Pitch;
	float MPU_Roll;
};

struct MPU6050_Quaternion {
    float q0, q1, q2, q3;
};

// Prototipos de Funcion
void MPU6050_Init(I2C_HandleTypeDef *i2cMPU);
void MPU6050_Write(uint8_t reg, uint8_t value);
uint8_t MPU6050_Read(void);
uint8_t MPU6050_ReadFIFO(void);
float MPU6050_getAx(void);
float MPU6050_getAy(void);
float MPU6050_getAz(void);
float MPU6050_getGx(void);
float MPU6050_getGy(void);
float MPU6050_getGz(void);
float MPU6050_getYaw(void);
float MPU6050_getPitch(void);
float MPU6050_getRoll(void);
float MPU6050_getTemperature(void);

#endif
