/**
 * @file    max31723.c
 * @brief   Driver for MAX31723 Temperature Sensor
 * @details 
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

#include "max31723.h"

/**** Functions ****/

int max31723_spi_init()
{
    // Initialize the SPI peripheral on the given port.
    int err;
    err = MXC_SPI_Init(MAX31723_SPI_REG, 
                       SPI_ROLE_MASTER, 
                       SPI_QUAD_MODE_OFF, 
                       SPI_NUM_PERIPHS, 
                       SPI_SS_POL_ACTIVE_HIGH, 
                       SPI_SPEED);
    
    // Set SPI bus configuration.
    MXC_SPI_SetMode(MAX31723_SPI_REG, SPI_MODE_3);
    MXC_SPI_SetDataSize(MAX31723_SPI_REG, 8);     // 8-bit character size.
    MXC_SPI_SetWidth(MAX31723_SPI_REG, SPI_WIDTH_STANDARD); // Width of data lines is
                                                            // standard MOSI/MISO; not quad.

    // Configure GPIO pins for SPI1.
    // Start with MSDK-provided config struct.
    mxc_gpio_cfg_t gpio_spi = gpio_cfg_spi1;
    // Modify for ss0 and 3.3V logic.
    gpio_spi.mask |= gpio_cfg_spi1_ss0.mask;
    gpio_spi.vssel = MXC_GPIO_VSSEL_VDDIOH;
    // Initiate configuration.
    MXC_GPIO_Config(&gpio_spi);

    // Write MAX31723 Config/Status register to reset to defaults.
    max31723_write_reg(0x00, 0x00);
    MXC_Delay(100);

    return err;
}

void max31723_write_reg(uint8_t reg, uint8_t data)
{
    // Set the Write bit (MSB) on register address.
    reg |= 0x80; 

    // Create transaction datagram (address + data).
    uint8_t tx_data[2] = {reg, data};
    
    // Create SPI request struct.
    mxc_spi_req_t spi_req = {
        .spi =          MAX31723_SPI_REG,
        .txData =       tx_data, // Note: The name of an array is already a pointer.
        .rxData =       NULL,
        .txLen =        2, 
        .rxLen =        0, // rx data on MISO is discarded if rxlen == 0
        .ssIdx =        0,
        .ssDeassert =   1,
        .txCnt =        0,
        .rxCnt =        0,
        .completeCB =   NULL
    };

    // Initiate transaction.
    MXC_SPI_MasterTransaction(&spi_req);
    return;
}


uint8_t max31723_read_reg(uint8_t reg) 
{
    int err;

    // Clear Write bit (MSB) from register address.
    reg &= ~0x80;

    // Create transaction datagram. Data is a "dummy" byte.
    uint8_t t_data[2] = {reg, 0xFF};

    // Initialize an empty receive array.
    uint8_t r_data[2] = {0x00, 0x00};
    
    // Create SPI request struct.
    mxc_spi_req_t r_req = {
        .spi           = MAX31723_SPI_REG,
        .txData        = t_data,
        .rxData        = r_data,
        .txLen         = 2,
        .rxLen         = 2,
        .ssIdx         = 0,
        .ssDeassert    = 1,
        .txCnt         = 0,
        .rxCnt         = 0,
        .completeCB    = NULL
    };

    // Initiate transaction.
    err = MXC_SPI_MasterTransaction(&r_req);
    if (err != E_NO_ERROR)
    {
        printf("Issue reading SPI peripheral: errno %d", err);
    }

    // Return read data (second byte of rx array).
    return r_data[1]; 
}