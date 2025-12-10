#ifndef __MAX31723_H
#define __MAX31723_H

/**
 * @file    max31723.h
 * @brief   Header file for MAX31723 SPI temperature sensor
 * @details SPI Init, read/write functions, register names/addresses
 * 
 */

/******************************************************************************
 *
 * Copyright (C) 2022-2023 Maxim Integrated Products, Inc. (now owned by 
 * Analog Devices, Inc.),
 * Copyright (C) 2023-2024 Analog Devices, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include "mxc_device.h"
#include "mxc_delay.h"
#include "spi.h"

/* **** Definitions **** */

// SPI Communication Config
#define SPI_SPEED           10000      // Bit Rate (Hz)
#define MAX31723_SPI_REG    MXC_SPI1    // Set to SPI port
#define SPI_NUM_PERIPHS     1

typedef enum {
    SPI_SS_POL_ACTIVE_LOW,
    SPI_SS_POL_ACTIVE_HIGH
} spi_ss_polarity_t;

typedef enum {
    SPI_ROLE_CLIENT,
    SPI_ROLE_MASTER
} spi_role_t;

typedef enum {
    SPI_QUAD_MODE_OFF,
    SPI_QUAD_MODE_ON
} spi_quad_mode_t;


// Register Bitfields
typedef enum {
    RES_9_BITS = 0,
    RES_10_BITS = (0x1 << 1),
    RES_11_BITS = (0x2 << 1),
    RES_12_BITS = (0x3 << 1)
} max31723_resolution_t;


/* **** Functions **** */

/**
 * @brief Initializes the SPI bus for communication with the MAX31723 sensor.
 *
 * This function configures the SPI bus for communication with the MAX31723. It
 * must be called before any other MAX31723 API functions. 
 *
 * The function also writes an initial value to the MAX31723's register
 * Configuration/Status Register to reset the part to power-up defaults, and
 * place the part in continuous conversion mode.
 *
 * Example usage: max31723_spi_init(MAX31723_SPI);
 *
 * @param spi A pointer to the SPI registers to be initialized. 
 *
 * @return 0 if the initialization is successful, or an error code otherwise.
 */
int max31723_spi_init(void);


 /**
 * @brief Reads a value from a specific register of the MAX31723 sensor.
 *
 * This function sends a read command to the MAX31723 via SPI. 
 *
 * Ensure that the SPI bus is initialized using max31723_spi_init() before
 * calling this function.
 *
 * @param reg The register address to read from. The MSB of this parameter is
 *            automatically cleared to indicate a read operation.
 *
 * @return The value read from the specified register.
 */
uint8_t max31723_read_reg(uint8_t reg);


 /**
 * @brief Writes a value to a specific register of the MAX31723 sensor.
 *
 * This function sends a write command to the MAX31723 sensor over SPI. 
 *
 * Ensure that the SPI bus is initialized using max31723_spi_init() before
 * calling this function.
 *
 * @param reg The register address to write to. The MSB of this parameter is
 *            automatically set to indicate a write operation.
 * @param w_data The data byte to write to the specified register.
 */
void max31723_write_reg(uint8_t reg, uint8_t w_data);

#endif