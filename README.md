# MAX31723 SPI Driver

## Description

This project provides a driver for the [MAX31723](https://www.analog.com/en/products/MAX31723.html) SPI temperature sensor. The driver includes functions to initialize the SPI bus, read, and write to the sensor's registers. 

This driver is designed to work with the MAX32650, but can be ported to other MAX32xxx microcontrollers with minimal additional configuration.

## Project Usage

This project only tracks application-specific driver files. Copy the repo's contents into a clean (Hello_World) project from the MaximSDK Examples folder.

Please refer to the [MSDK User Guide](https://analogdevicesinc.github.io/msdk/USERGUIDE/) for instructions on building, flashing, and debugging with Maxim microcontrollers.

For working with the MAX31723, it is recommended to use the [MAX31723PMB1 evaluation board](https://www.analog.com/en/resources/evaluation-hardware-and-software/evaluation-boards-kits/max31723pmb1.html).

## Required Connections

Connect the MAX31723PMB1 temperature sensor board to the SPI1 pins on the MAX32650. The driver is configured by default to use SS0.

> [!WARNING]
> Disconnect / turn off power to the microcontroller **before** making connections!

## Expected Output

The Console UART of the device will output these messages:

```
Register 0x0: 0x0 
Register 0x1: 0x80 
Register 0x2: 0x17 
Register 0x3: 0xFF 
Register 0x4: 0xFF 
Register 0x5: 0xFF 
Register 0x6: 0xFF 

Example Complete.
```

