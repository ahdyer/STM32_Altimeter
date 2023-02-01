/*
 * bmp388.c
 *
 *  Created on: 12 Jan 2023
 *      Author: alexd
 */
#include "bmp388.h"




HAL_StatusTypeDef BMP388_Init(I2C_HandleTypeDef *I2C_Bus, struct BMP388_Coeffs *calCoeffs){
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

	// Could be a problem here as the I2C read command only wants a uint8_t pointer,
	//	but unsure how that allows you to read more than one byte
	// Read all of the coefficients
		uint8_t unifiedData[21];

	status = HAL_I2C_Mem_Read(I2C_Bus, BMP388_ADDRESS, T1_REG, 1, unifiedData, 21, Timeout);
		if (status != HAL_OK){
				return status;
		}

	calCoeffs->nvm_par_t1 = unifiedData[(T1_REG - REG_OFFSET + 1)] << 8 | unifiedData[(T1_REG - REG_OFFSET)];
	calCoeffs->nvm_par_t2 = unifiedData[(T2_REG - REG_OFFSET + 1)] << 8 | unifiedData[(T2_REG - REG_OFFSET)];
	calCoeffs->nvm_par_t3 = unifiedData[(T3_REG - REG_OFFSET)];
	calCoeffs->nvm_par_p1 = unifiedData[(P1_REG - REG_OFFSET + 1)] << 8 | unifiedData[(P1_REG - REG_OFFSET)];
	calCoeffs->nvm_par_p2 = unifiedData[(P2_REG - REG_OFFSET + 1)] << 8 | unifiedData[(P2_REG - REG_OFFSET)];
	calCoeffs->nvm_par_p3 = unifiedData[(P3_REG - REG_OFFSET)];
	calCoeffs->nvm_par_p4 = unifiedData[(P4_REG - REG_OFFSET)];
	calCoeffs->nvm_par_p5 = unifiedData[(P5_REG - REG_OFFSET + 1)] << 8 | unifiedData[(P5_REG - REG_OFFSET)];
	calCoeffs->nvm_par_p6 = unifiedData[(P6_REG - REG_OFFSET + 1)] << 8 | unifiedData[(P6_REG - REG_OFFSET)];
	calCoeffs->nvm_par_p7 = unifiedData[(P7_REG - REG_OFFSET)];
	calCoeffs->nvm_par_p8 = unifiedData[(P8_REG - REG_OFFSET)];
	calCoeffs->nvm_par_p9 = unifiedData[(P9_REG - REG_OFFSET + 1)] << 8 | unifiedData[(P9_REG - REG_OFFSET)];
	calCoeffs->nvm_par_p10 = unifiedData[(P10_REG - REG_OFFSET)];
	calCoeffs->nvm_par_p11 = unifiedData[(P11_REG - REG_OFFSET)];

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

HAL_StatusTypeDef BMP388_Read(I2C_HandleTypeDef *I2C_Bus, struct BMP388_Outputs *data, struct BMP388_Coeffs *calCoeffs){
	HAL_StatusTypeDef status = HAL_ERROR;
	uint8_t unifiedData[6];
	int64_t uncalibratedTemp;
	uint64_t uncalibratedPres;
	status = HAL_I2C_Mem_Read(I2C_Bus, BMP388_ADDRESS, T0_Data_Reg, 1, unifiedData, 6, Timeout);
	if (status != HAL_OK){
		return status;
	}

	uncalibratedPres = unifiedData[2] << 16 | unifiedData[1] << 8 | unifiedData[0];
	uncalibratedTemp = unifiedData[5] << 16 | unifiedData[4] << 8 | unifiedData[3];


	data->temprature = BMP388_Calibrate_Temprature(calCoeffs, uncalibratedTemp);
	data->pressure = BMP388_Calibrate_Pressure(calCoeffs, uncalibratedPres);

	return HAL_OK;
}

int64_t BMP388_Calibrate_Temprature(struct BMP388_Coeffs *calCoeffs, int64_t uncalibratedTemp){
	double T1Data;
	double T2Data;

	T1Data = (double)(uncalibratedTemp - calCoeffs->par_t1);
	T2Data = (double)(T1Data * calCoeffs->par_t2);
	calCoeffs->temprature = T2Data + ((T1Data * T1Data) * calCoeffs->nvm_par_t3);

	if (calCoeffs->temprature < -40.0f){
		calCoeffs->temprature = -40.0f;
	}
	if (calCoeffs->temprature > 85.0f){
		calCoeffs->temprature = 85.0f;
	}

	return (int64_t)calCoeffs->temprature;
}

uint64_t BMP388_Calibrate_Pressure(struct BMP388_Coeffs *calCoeffs, uint64_t uncalibratedPres){
	double dataOut1;
	double dataOut2;
	double dataOut3;
	double compensatedPressure;

	dataOut1 =  calCoeffs->par_p6 * calCoeffs->temprature;
	dataOut1 += calCoeffs->par_p7 * pow(calCoeffs->temprature, (float)2.0);
	dataOut1 += calCoeffs->par_p8 * pow(calCoeffs->temprature, (float)3.0);
	dataOut1 += calCoeffs->par_p5;

	dataOut2 =  calCoeffs->par_p2 * calCoeffs->temprature;
	dataOut2 += calCoeffs->par_p3 * pow(calCoeffs->temprature, (float)2.0);
	dataOut2 += calCoeffs->par_p4 * pow(calCoeffs->temprature, (float)3.0);
	dataOut2 += calCoeffs->par_p1;
	dataOut2 *= uncalibratedPres;

	dataOut3 =  calCoeffs->par_p10 * calCoeffs->temprature;
	dataOut3 += calCoeffs->par_p9;
	dataOut3 *= pow((double)uncalibratedPres, 2);
	dataOut3 += calCoeffs->par_p11 * pow((double)uncalibratedPres, 3);

	compensatedPressure = dataOut1 + dataOut2 + dataOut3;

	if (compensatedPressure < 30000.0f){
		compensatedPressure = 30000.0f;
	}
	if (compensatedPressure > 125000.0f){
		compensatedPressure = 125000.0f;
	}

	return (uint64_t)compensatedPressure;

}

