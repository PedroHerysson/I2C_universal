/*
 * mlx90614.h
 *
 *  Created on: Oct 13, 2026
 *      Author: eduar
 */

#ifndef MLX90614_H_
#define MLX90614_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

/* Endereço e Registradores */
#define MLX90614_ADDRESS    (0x5A << 1)
#define MLX90614_TOBJ1      0x07

/* Protótipos das Funções da Biblioteca */
HAL_StatusTypeDef MLX90614_Init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef MLX90614_ReadTemperature(I2C_HandleTypeDef *hi2c, float *temp_celsius);
HAL_StatusTypeDef MLX90614_ReadAndPrint(I2C_HandleTypeDef *hi2c, UART_HandleTypeDef *huart);

#ifdef __cplusplus
}
#endif

#endif /* MLX90614_H_ */
