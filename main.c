/**
 * @file    main.c
 * @brief   MAX31723 Demo
 * @details This example demonstrates usage of the MAX31723 driver API for
 *          temperature sensing.
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

/* **** Includes **** */
#include <stdio.h>
#include <stdint.h>
#include "mxc_delay.h"
#include "max31723.h"
#include "led.h"

/* **** Definitions **** */

/* **** Globals **** */

/* **** Functions **** */

/* ************************************************************************** */
int main(void)
{
    // Initialize communication with the MAX31723
    if (max31723_spi_init() != E_NO_ERROR) {
        printf("\nSPI INITIALIZATION ERROR\n");
        while (1) {}
    }
    
    // Get register values from device.
    uint8_t reg_data;
    for(int i = 0; i < 7; i++) 
    {
        reg_data = max31723_read_reg(i);
        printf("Register 0x%X: 0x%X \n", i, reg_data);
    }

    printf("\nExample Complete.\n");

    // End examples with infinite while loop. (No Operating System!)
    while(1);
}
