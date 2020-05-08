/*
 * CS43.c
 *
 *  Created on: 7 maj 2020
 *      Author: kaper
 */


#include "CS43.h"
#include "stm32f4xx_hal.h"

void CS43_Start(I2C_HandleTypeDef* hi2c1)
{
	uint8_t data[1];
	data[0] = 0x01;
	HAL_StatusTypeDef status;

	//__HAL_UNLOCK(&hi2s3);
	//__HAL_I2S_ENABLE(&hi2s3);

	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET);

	while ((status = HAL_I2C_Mem_Write(hi2c1, 0x94, 0x02, 1, data, 1, 100)) != HAL_OK);

	data[0] = 0x99;
	while ((status = HAL_I2C_Mem_Write(hi2c1, 0x94, 0x00, 1, data, 1, 100)) != HAL_OK);
	data[0] = 0x99;
	while ((status = HAL_I2C_Mem_Write(hi2c1, 0x94, 0x00, 1, data, 1, 100)) != HAL_OK);

	data[0]=0x80;
	while ((status = HAL_I2C_Mem_Write(hi2c1, 0x94, 0x47, 1, data, 1, 100)) != HAL_OK);

	while ((status = HAL_I2C_Mem_Read(hi2c1, 0x94, 0x32, 1, data, 1, 100)) != HAL_OK);
	data[0] |= 0x80;
	while ((status = HAL_I2C_Mem_Write(hi2c1, 0x94, 0x32, 1, data, 1, 100)) != HAL_OK);

	while ((status = HAL_I2C_Mem_Read(hi2c1, 0x94, 0x32, 1, data, 1, 100)) != HAL_OK);
	data[0] &= ~(0x80);
	while ((status = HAL_I2C_Mem_Write(hi2c1, 0x94, 0x32, 1, data, 1, 100)) != HAL_OK);

	data[0] = 0;
	while ((status = HAL_I2C_Mem_Write(hi2c1, 0x94, 0x00, 1, data, 1, 100)) != HAL_OK);

	data[0] = 0x9E;
	while ((status = HAL_I2C_Mem_Write(hi2c1, 0x94, 0x02, 1, data, 1, 100)) != HAL_OK);

	data[0] = 175;
	while ((status = HAL_I2C_Mem_Write(hi2c1, 0x94, 0x04, 1, data, 1, 100)) != HAL_OK);

	data[0]=(1<<7);
	while ((status = HAL_I2C_Mem_Write(hi2c1, 0x94, 0x05, 1, data, 1, 100)) != HAL_OK);

	while ((status = HAL_I2C_Mem_Read(hi2c1, 0x94, 0x06, 1, data, 1, 100)) != HAL_OK);
	data[0] &= (1 << 5);
	data[0] &= ~(1 << 7);
	data[0] &= ~(1 << 6);
	data[0] &= ~(1 << 4);
	data[0] &= ~(1 << 2);
	data[0] |= (1 << 2);
	data[0] |=  (3 << 0);
	while ((status = HAL_I2C_Mem_Write(hi2c1, 0x94, 0x06, 1, data, 1, 100)) != HAL_OK);

	data[0]=0x02;
	while ((status = HAL_I2C_Mem_Write(hi2c1, 0x94, 0x0E, 1, data, 1, 100)) != HAL_OK);

	data[0] = 4;
	while ((status = HAL_I2C_Mem_Write(hi2c1, 0x94, 0x0e, 1, data, 1, 100)) != HAL_OK);

	data[0] = 0x00;
	while ((status = HAL_I2C_Mem_Write(hi2c1, 0x94, 0x14, 1, data, 1, 100)) != HAL_OK);
	while ((status = HAL_I2C_Mem_Write(hi2c1, 0x94, 0x15, 1, data, 1, 100)) != HAL_OK);
	while ((status = HAL_I2C_Mem_Write(hi2c1, 0x94, 0x1A, 1, data, 1, 100)) != HAL_OK);
	while ((status = HAL_I2C_Mem_Write(hi2c1, 0x94, 0x1B, 1, data, 1, 100)) != HAL_OK);

	data[0]= 0xAF;
	while ((status = HAL_I2C_Mem_Write(hi2c1, 0x94, 0x04, 1, data, 1, 100)) != HAL_OK);


}
