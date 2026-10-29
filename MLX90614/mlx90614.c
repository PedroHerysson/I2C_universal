/*
 * mlx90614.c
 *
 *  Created on: Oct 13, 2026
 *      Author: eduar
 */

#include "mlx90614.h"
#include <stdio.h>
#include <string.h>

/**
 * @brief  Verifica se o sensor MLX90614 está conectado no barramento I2C informado.
 * @param  hi2c: Ponteiro para a instância I2C (ex: &hi2c1, &hi2c2)
 * @retval HAL_StatusTypeDef
 */
HAL_StatusTypeDef MLX90614_Init(I2C_HandleTypeDef *hi2c)
{
    return HAL_I2C_IsDeviceReady(hi2c, MLX90614_ADDRESS, 3, 100);
}

/**
 * @brief  Lê a temperatura do Objeto 1 (Tobj1) do sensor MLX90614.
 * @param  hi2c: Ponteiro para a instância I2C
 * @param  temp_celsius: Ponteiro para armazenar o valor da temperatura calculada
 * @retval HAL_StatusTypeDef
 */
HAL_StatusTypeDef MLX90614_ReadTemperature(I2C_HandleTypeDef *hi2c, float *temp_celsius)
{
    uint8_t buf[3] = {0};
    uint16_t raw_temp = 0;
    HAL_StatusTypeDef status;

    // Lê 3 bytes (LSB, MSB, PEC) da RAM do sensor
    status = HAL_I2C_Mem_Read(hi2c, MLX90614_ADDRESS, MLX90614_TOBJ1,
                              I2C_MEMADD_SIZE_8BIT, buf, 3, 100);

    if (status == HAL_OK) {
        // Conversão em Little Endian e aplicação da fórmula do datasheet: T = (Raw * 0.02) - 273.15
        raw_temp = (buf[1] << 8) | buf[0];
        *temp_celsius = (raw_temp * 0.02f) - 273.15f;
    }

    return status;
}

/**
 * @brief  Lê a temperatura e envia a mensagem formatada via UART.
 * @param  hi2c: Ponteiro para a instância I2C
 * @param  huart: Ponteiro para a instância UART utilizada para envio das mensagens
 * @retval HAL_StatusTypeDef
 */
HAL_StatusTypeDef MLX90614_ReadAndPrint(I2C_HandleTypeDef *hi2c, UART_HandleTypeDef *huart)
{
    float temp_celsius = 0.0f;
    char msg[64];
    HAL_StatusTypeDef status;

    status = MLX90614_ReadTemperature(hi2c, &temp_celsius);

    if (status == HAL_OK) {
        sprintf(msg, "Temp do Objeto: %.2f C\r\n", temp_celsius);
        HAL_UART_Transmit(huart, (uint8_t*)msg, strlen(msg), 100);
    } else {
        sprintf(msg, "Erro na leitura do sensor. Status: %d\r\n", status);
        HAL_UART_Transmit(huart, (uint8_t*)msg, strlen(msg), 100);
    }

    return status;
}
