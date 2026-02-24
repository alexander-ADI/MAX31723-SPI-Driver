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
#define SPI_SPEED           1000000      // Bit Rate (Hz)
#define MAX31723_SPI_PORT    MXC_SPI1    // Set to SPI port
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


/*** Registers ***/

// Note: These are read addresses. Change MSB (bit 7) = 1 for writes.
#define MAX31723_REG_CONFIG_STATUS  ((uint8_t)0x00)
#define MAX31723_REG_TEMP_LSB       ((uint8_t)0x01)
#define MAX31723_REG_TEMP_MSB       ((uint8_t)0x02)
#define MAX31723_REG_THIGH_LSB      ((uint8_t)0x03)
#define MAX31723_REG_THIGH_MSB      ((uint8_t)0x04)
#define MAX31723_REG_TLOW_LSB       ((uint8_t)0x05)
#define MAX31723_REG_TLOW_MSB       ((uint8_t)0x06)

/*** Register Bitfields ***/

// MEMW: Memory write bit (Bit 6)
// 0 = RAM, 1 = EEPROM
typedef enum {
    MAX31723_MEMWRITE_RAM = (0 << 6),
    MAX31723_MEMWRITE_EEPROM = (1 << 6)
} max31723_memwrite_t;

// NVB: Nonvolatile memory busy flag (Bit 5)
// 0 = NVM not busy.
// 1 = NVM busy, there is a write to EEPROM memory cell in progress.
typedef enum {
    MAX31723_NVM_NOT_BUSY = (0 << 5),
    MAX31723_NVM_BUSY     = (1 << 5)
} max41723_nvmbusy_t;

// 1SHOT: One-shot temperature conversion bit (Bit 4)
// 0 = Disable, 1 = Enable one-shot
typedef enum {
    MAX31723_ONESHOT_DISABLE = (0 << 4),
    MAX31723_ONESHOT_ENABLE = (1 << 4)
} max31723_oneshot_t;

// TM: Thermostat operating mode (Bit 3)
// 0 = Comparator, 1 = Interrupt
typedef enum {
    MAX31723_THERMOSTAT_COMP_MODE = (0 << 3),
    MAX31723_THERMOSTAT_INTR_MODE = (1 << 3)
} max31723_thermostat_t;

// R1:R0: Resolution bits (Bits 2:1)
// 00 = 9 bits, 01 = 10 bits, 10 = 11 bits, 11 = 12 bits
typedef enum {
    MAX31723_RES_9_BITS = 0,
    MAX31723_RES_10_BITS = (0x1 << 1),
    MAX31723_RES_11_BITS = (0x2 << 1),
    MAX31723_RES_12_BITS = (0x3 << 1)
} max31723_resolution_t;

// SD: Shutdown bit (Bit 0)
// 0 = Continuous, 1 = 1-shot (shutdown)
typedef enum {
    MAX31723_SHUTDOWN_CONVERT_CONTINUOUS = 0,
    MAX31723_SHUTDOWN_CONVERT_1SHOT = 1
} max31723_shutdown_t;


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

/**
 * @brief Reads a temperature value from the MAX31723 sensor, using LSB address as target.
 *
 * This function reads MSB and LSB temperature registers from the MAX31723, given an LSB.
 * It combines MSB and LSB registers to form a complete 16-bit temperature value.
 *
 * @param lsb_address The LSB register address to read the temperature field from.
 *
 * @return The 16-bit temperature value read from the sensor.
 */
uint16_t max31723_read_temp_bytes(uint8_t lsb_address);

/**
 * @brief Writes a temperature value to a MAX31723 temperature field, using LSB address as target.
 *
 * This function writes the given 16-bit temperature value to the MAX31723 temperature registers specified by the provided LSB address.
 * The value is split into MSB and LSB and written to the respective registers.
 *
 * @param lsb_address The LSB register address of the temperature value to target.
 * @param temp_bytes The 16-bit temperature value to write to the sensor.
 */
void max31723_write_temp_bytes(uint8_t lsb_address, uint16_t temp_bytes);

#endif