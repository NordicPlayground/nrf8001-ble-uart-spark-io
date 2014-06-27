/* Copyright (c) 2014, Nordic Semiconductor ASA
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
 
#ifndef NRF8001_BLE_UART_SPARK_IO_H__
#define NRF8001_BLE_UART_SPARK_IO_H__

#include <application.h>
#include "uart_aci.h"

/**
* Main user interface for the Nordic Semiconductor nRF8001 BLE UART library for the Spark Core
* All user functions are defined here, no further involvement with the library should be 
* necessary.
*
* 
* 
* The default pin mappings for the SPI connection to the nRF8001 is as follows:
* 
*
* nrf8001		- Spark Core	-	Explanation
*
* MOSI 			- A5					- SPI Master Out, Slave In
* MISO			- A4					- SPI Master In, Slave Out
* SCK				- A3 					- SPI Bus clock (may be represented as CLK on your board, depending on version)
* REQN			- D4					- SPI Master request line
* RDYN			- D3					- SPI Slave ready line
* RESET			- D2					- nRF8001 reset signal (triggered in the library setup)
* VIN				- 3V3					- nRF8001 power 
* GND				- GND					- Common ground
*/








/**
* Setup routine for all BLE UART related functionality
* Should be executed in the setup() function in the project sketch
* Sets the default pin configuration.
*/
void nRF8001BleUartSetup(void);

/**
* Setup routing for all BLE UART related functionality
* Should be executed in the setup() function in the project sketch
* Allows the user to set up a custom pin configuration for the nRF8001 interface. 
* 
* Example usage: 
* nRF8001BleUartSetup(A5, A4, A3, D4, D3, D2); // default configuration
*
* NOTE: Not all pin configurations are guaranteed to work, and it is recommended to 
* stick to the default configuration.
*/
void nRF8001BleUartSetup(	uint8_t pin_mosi, 
													uint8_t pin_miso, 
													uint8_t pin_sck, 
													uint8_t pin_reqn, 
													uint8_t pin_rdyn, 
													uint8_t pin_reset);

													
													
													
													
													
													
													
					


					
/**
* Loop function, taking care of all BLE UART communication
* All incoming BLE events and UART messages are processed in this function.
* Should be executed in the loop() function in the project sketch
*/
void nRF8001BleUartLoop(void);






/**
* Public interface for sending data through the BLE UART
*/
bool nRF8001BleUartTx(uint8_t *buffer, uint8_t buffer_len);










/**
* Function to set the device's on-air name, that will be visible for any devices
* that wish to connect to the Spark Core.
* The device name may not be longer than 16 bytes.
*/
void nRF8001BleUartNameSet(const char* device_name, uint8_t name_length);











/**
* Callback function for incoming UART messages
* Called with the contents and length of the message 
* 
* Implemented by user
*/
void nRF8001BleUartRx(uint8_t *buffer, uint8_t len);




















#endif /*NRF8001_BLE_UART_SPARK_IO_H__*/