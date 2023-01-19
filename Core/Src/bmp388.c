/*
 * bmp388.c
 *
 *  Created on: 12 Jan 2023
 *      Author: alexd
 */
#include "bmp388.h"

HAL_StatusTypeDef BMP388_Init(I2C_HandleTypeDef *I2C_Bus){
	// See if the device requires a soft reset
	// see if the device needs to come out of sleep mode
	// read the device ID Reg 0x00 and check it = 50
	// Configure the device
	HAL_StatusTypeDef status = HAL_ERROR;
	uint8_t chipID = 0;
	status = HAL_I2C_Mem_Read(I2C_Bus, BMP388_ADDRESS, CHIP_ID_REG, 1, &chipID, 1, 50);
	if (status != HAL_OK){
		return status;
	}
	if (chipID != 0x50) {
		return HAL_ERROR;
	}
	// Flash Status LED ! For now this is the buzzer
	HAL_GPIO_WritePin(GPIOA, Buz_Trig_Pin, 1);
	HAL_Delay(50);
	HAL_GPIO_WritePin(GPIOA, Buz_Trig_Pin, 0);

	// Enable normal power mode
	status = HAL_I2C_Mem_Write(I2C_Bus, BMP388_ADDRESS, PWR_CTRL, 1, (uint8_t*)NORMAL_MODE, 1, 50);
		if (status != HAL_OK){
			return status;
		}

	// Set measurement resolution
	status = HAL_I2C_Mem_Write(I2C_Bus, BMP388_ADDRESS, OSR, 1, (uint8_t*)(OSR_P & OSR4_T), 1, 50);
		if (status != HAL_OK){
			return status;
		}
	// set IIR filter coefficients
	status = HAL_I2C_Mem_Write(I2C_Bus, BMP388_ADDRESS, CONFIG, 1, (uint8_t*)IIR_COEF_3, 1, 50);
		if (status != HAL_OK){
			return status;
		}

	return HAL_OK;
}

