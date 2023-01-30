/*
 * bmp388.c
 *
 *  Created on: 12 Jan 2023
 *      Author: alexd
 */
#include "bmp388.h"




HAL_StatusTypeDef BMP388_Init(I2C_HandleTypeDef *I2C_Bus, struct Coeffs *calCoeffs){
	// See if the device requires a soft reset
	// see if the device needs to come out of sleep mode
	// read the device ID Reg 0x00 and check it = 50
	// Configure the device
	HAL_StatusTypeDef status = HAL_ERROR;
	uint8_t chipID = 0;

	status = HAL_I2C_Mem_Read(I2C_Bus, BMP388_ADDRESS, CHIP_ID_REG, 1, &chipID, 1, Timeout);
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
	status = HAL_I2C_Mem_Write(I2C_Bus, BMP388_ADDRESS, PWR_CTRL, 1, (uint8_t*)NORMAL_MODE, 1, Timeout);
		if (status != HAL_OK){
			return status;
		}

	// Set measurement resolution
	status = HAL_I2C_Mem_Write(I2C_Bus, BMP388_ADDRESS, OSR, 1, (uint8_t*)(OSR_P & OSR4_T), 1, Timeout);
		if (status != HAL_OK){
			return status;
		}
	// set IIR filter coefficients
	status = HAL_I2C_Mem_Write(I2C_Bus, BMP388_ADDRESS, CONFIG, 1, (uint8_t*)IIR_COEF_3, 1, Timeout);
		if (status != HAL_OK){
			return status;
		}
	// Read all of the coefficients
	status = HAL_I2C_Mem_Read(I2C_Bus, BMP388_ADDRESS, NVM_PAR_T1, 2, &calCoeffs->nvm_par_t1, 2, Timeout);
		if (status != HAL_OK){
				return status;
		}
	status = HAL_I2C_Mem_Read(I2C_Bus, BMP388_ADDRESS, NVM_PAR_T2, 2, &calCoeffs->nvm_par_t2, 2, Timeout);
		if (status != HAL_OK){
				return status;
		}
	status = HAL_I2C_Mem_Read(I2C_Bus, BMP388_ADDRESS, NVM_PAR_T3, 1, &calCoeffs->nvm_par_t3, 1, Timeout);
		if (status != HAL_OK){
				return status;
		}
	status = HAL_I2C_Mem_Read(I2C_Bus, BMP388_ADDRESS, NVM_PAR_P1, 2, &calCoeffs->nvm_par_p1, 2, Timeout);
		if (status != HAL_OK){
				return status;
		}
	status = HAL_I2C_Mem_Read(I2C_Bus, BMP388_ADDRESS, NVM_PAR_P2, 2, &calCoeffs->nvm_par_p2, 2, Timeout);
		if (status != HAL_OK){
				return status;
		}
	status = HAL_I2C_Mem_Read(I2C_Bus, BMP388_ADDRESS, NVM_PAR_P3, 1, &calCoeffs->nvm_par_p3, 1, Timeout);
		if (status != HAL_OK){
				return status;
		}
	status = HAL_I2C_Mem_Read(I2C_Bus, BMP388_ADDRESS, NVM_PAR_P4, 1, &calCoeffs->nvm_par_p4, 1, Timeout);
		if (status != HAL_OK){
				return status;
		}
	status = HAL_I2C_Mem_Read(I2C_Bus, BMP388_ADDRESS, NVM_PAR_P5, 2, &calCoeffs->nvm_par_p5, 2, Timeout);
		if (status != HAL_OK){
				return status;
		}
	status = HAL_I2C_Mem_Read(I2C_Bus, BMP388_ADDRESS, NVM_PAR_P6, 2, &calCoeffs->nvm_par_p6, 2, Timeout);
		if (status != HAL_OK){
				return status;
		}
	status = HAL_I2C_Mem_Read(I2C_Bus, BMP388_ADDRESS, NVM_PAR_P7, 1, &calCoeffs->nvm_par_p7, 1, Timeout);
		if (status != HAL_OK){
				return status;
		}
	status = HAL_I2C_Mem_Read(I2C_Bus, BMP388_ADDRESS, NVM_PAR_P8, 1, &calCoeffs->nvm_par_p8, 1, Timeout);
		if (status != HAL_OK){
				return status;
		}
	status = HAL_I2C_Mem_Read(I2C_Bus, BMP388_ADDRESS, NVM_PAR_P9, 2, &calCoeffs->nvm_par_p9, 2, Timeout);
		if (status != HAL_OK){
				return status;
		}
	status = HAL_I2C_Mem_Read(I2C_Bus, BMP388_ADDRESS, NVM_PAR_P10, 1, &calCoeffs->nvm_par_p10, 1, Timeout);
		if (status != HAL_OK){
				return status;
		}
	status = HAL_I2C_Mem_Read(I2C_Bus, BMP388_ADDRESS, NVM_PAR_P11, 1, &calCoeffs->nvm_par_p11, 1, Timeout);
		if (status != HAL_OK){
				return status;
		}


	// Watch out, some of these that go over e+38 might need to be doubles
	calCoeffs->par_t1 = calCoeffs->nvm_par_t1 / pow(2, -8);
	calCoeffs->par_t2 = calCoeffs->nvm_par_t2 / pow(2, 30);
	calCoeffs->par_t3 = calCoeffs->nvm_par_t3 / pow(2, 48);

	calCoeffs->par_p1 = calCoeffs->nvm_par_p1 / pow(2, 20);
	calCoeffs->par_p2 = calCoeffs->nvm_par_p2 / pow(2, 29);
	calCoeffs->par_p3 = calCoeffs->nvm_par_p3 / pow(2, 32);
	calCoeffs->par_p4 = calCoeffs->nvm_par_p4 / pow(2, 37);
	calCoeffs->par_p5 = calCoeffs->nvm_par_p5 / pow(2, -3);
	calCoeffs->par_p6 = calCoeffs->nvm_par_p6 / pow(2, 6);
	calCoeffs->par_p7 = calCoeffs->nvm_par_p7 / pow(2, 8);
	calCoeffs->par_p8 = calCoeffs->nvm_par_p8 / pow(2, 15);
	calCoeffs->par_p9 = calCoeffs->nvm_par_p9 / pow(2, 48);
	calCoeffs->par_p10 = calCoeffs->nvm_par_p10 / pow(2, 48);
	calCoeffs->par_p11 = calCoeffs->nvm_par_p11 / pow(2, 65);

	return HAL_OK;
}

