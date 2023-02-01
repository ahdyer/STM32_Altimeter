
#ifndef BMP388
#define BMP388

#include "stm32f0xx_hal.h"
#include "gpio_pin_names.h"
#include "math.h"
#include "stdlib.h"

#define Timeout  		50

#define BMP388_ADDRESS 	(uint8_t) (0x76 << 1)
#define CHIP_ID_REG 	(uint8_t)0x00
#define PWR_CTRL 		(uint8_t)0x1B
#define ODR				(uint8_t)0x1D
#define OSR 			(uint8_t)0x1C
#define CONFIG			(uint8_t)0x1F
#define REG_OFFSET  	(uint8_t)0x31
#define T1_REG			(uint8_t)0x31
#define T2_REG			(uint8_t)0x33
#define T3_REG			(uint8_t)0x35
#define P1_REG			(uint8_t)0x36
#define P2_REG			(uint8_t)0x38
#define P3_REG			(uint8_t)0x3A
#define P4_REG			(uint8_t)0x3B
#define P5_REG			(uint8_t)0x3C
#define P6_REG			(uint8_t)0x3E
#define P7_REG			(uint8_t)0x40
#define P8_REG			(uint8_t)0x41
#define P9_REG			(uint8_t)0x42
#define P10_REG			(uint8_t)0x44
#define P11_REG			(uint8_t)0x45

#define T0_Data_Reg		(uint8_t)0x04

#define NORMAL_MODE 	(uint8_t)0b110011
#define OSR_P 			(uint8_t)0b011						// pressure over sampling = x 8
#define OSR4_T			(uint8_t)0b000000					// temperature over sampling = x1
#define IIR_COEF_3		(uint8_t)0b0010
// IIR Filter 2

struct BMP388_Coeffs{
	// Temperature coefficients
	uint16_t 	nvm_par_t1;
	uint16_t 	nvm_par_t2;
	int8_t 		nvm_par_t3;
	double 		par_t1;
	double 		par_t2;
	double 		par_t3;
	double 	temprature;
	// Pressure coefficients
	int16_t 	nvm_par_p1;
	int16_t 	nvm_par_p2;
	int8_t 		nvm_par_p3;
	int8_t 		nvm_par_p4;
	uint16_t 	nvm_par_p5;
	uint16_t 	nvm_par_p6;
	int8_t 		nvm_par_p7;
	int8_t 		nvm_par_p8;
	int16_t 	nvm_par_p9;
	int8_t 		nvm_par_p10;
	int8_t 		nvm_par_p11;

	double 		par_p1;
	double 		par_p2;
	double 		par_p3;
	double 		par_p4;
	double 		par_p5;
	double 		par_p6;
	double 		par_p7;
	double 		par_p8;
	double 		par_p9;
	double 		par_p10;
	double 		par_p11;
};

struct BMP388_Outputs{
	// Temperature coefficients
	uint64_t pressure;
	int64_t temprature;
};


// Functions
HAL_StatusTypeDef BMP388_Read(I2C_HandleTypeDef *I2C_Bus, struct BMP388_Outputs *data, struct BMP388_Coeffs *calCoeffs);

int64_t BMP388_Calibrate_Temprature(struct BMP388_Coeffs *calCoeffs, int64_t uncalibratedTemp);

uint64_t BMP388_Calibrate_Pressure(struct BMP388_Coeffs *calCoeffs, uint64_t uncalibratedPres);

#endif
