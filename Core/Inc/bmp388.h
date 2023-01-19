
#ifndef BMP388
#define BMP388

#include "stm32f0xx_hal.h"
#include "gpio_pin_names.h"

#define BMP388_ADDRESS 	(uint8_t) (0x76 << 1)
#define CHIP_ID_REG 	(uint8_t)0x00
#define PWR_CTRL 		(uint8_t)0x1B
#define ODR				(uint8_t)0x1D
#define OSR 			(uint8_t)0x1C
#define CONFIG			(uint8_t)0x1F

#define NORMAL_MODE 	(uint8_t)0b110011
#define OSR_P 			(uint8_t)0b011						// pressure over sampling = x 8
#define OSR4_T			(uint8_t)0b000000					// temperature over sampling = x1
#define IIR_COEF_3		(uint8_t)0b0010
// IIR Filter 2


// Functions

#endif
